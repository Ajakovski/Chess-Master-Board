/*
 * game/game.c — Chess board game logic
 *
 * State machine overview:
 *
 *  GP_BOOT → GP_WAIT_POSITION → GP_SELECT_SIDE → GP_PLAYING
 *             (32 pieces         (P1=White or       ↕
 *              present?)          P2=Black)       GP_GAME_OVER
 *                                               GP_SLEEPING ←→ GP_PLAYING
 */

#include "game.h"
#include "config.h"
#include "types.h"
#include "sensors.h"
#include "leds.h"
#include "display.h"
#include "buttons.h"
#include "battery.h"
#include "board.h"
#include "rules.h"
#include "uci.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_attr.h"           /* RTC_DATA_ATTR */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "game";

/* ============================================================
 * RTC-RETAINED GAME STATE  (survives deep sleep)
 * ============================================================ */
RTC_DATA_ATTR static uint32_t     rtc_magic;
RTC_DATA_ATTR static char         rtc_fen[128];
RTC_DATA_ATTR static int32_t      rtc_clock_ms[2];
RTC_DATA_ATTR static sf_side_t    rtc_sf_side;
RTC_DATA_ATTR static char         rtc_moves[MAX_HALF_MOVES][6];
RTC_DATA_ATTR static int          rtc_move_count;
RTC_DATA_ATTR static uint8_t      rtc_active_color;

/* ============================================================
 * SHARED GAME STATE
 * ============================================================ */
static game_state_t  s_gs;
static SemaphoreHandle_t s_mutex;

QueueHandle_t g_button_queue;

/* ============================================================
 * LED COLOUR CONSTANTS (from config.h macros)
 * ============================================================ */
static const rgb_t COL_MOVE_FROM   = LED_COLOR_MOVE_FROM;
static const rgb_t COL_SF_FROM     = LED_COLOR_SF_FROM;
static const rgb_t COL_SF_TO       = LED_COLOR_SF_TO;
static const rgb_t COL_VALID_DST   = LED_COLOR_VALID_DST;
static const rgb_t COL_INVALID     = LED_COLOR_INVALID;
static const rgb_t COL_CHECK       = LED_COLOR_CHECK;
static const rgb_t COL_CONFIRM_F   = LED_COLOR_CONFIRM_FROM;
static const rgb_t COL_CONFIRM_T   = LED_COLOR_CONFIRM_TO;

/* ============================================================
 * HELPERS
 * ============================================================ */

static inline void gs_lock  (void) { xSemaphoreTake(s_mutex, portMAX_DELAY); }
static inline void gs_unlock(void) { xSemaphoreGive(s_mutex); }

static inline uint32_t now_ms(void)
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

static int popcount64(uint64_t v)
{
    return __builtin_popcountll(v);
}

static uint64_t board_expected(const board_t *b)
{
    return board_occupied(b);
}

static uint64_t sq_removed(uint64_t expected, uint64_t actual)
{
    return expected & ~actual;
}

static uint64_t sq_added(uint64_t expected, uint64_t actual)
{
    return ~expected & actual;
}

/* ============================================================
 * LED BUFFER MANAGEMENT
 * ============================================================ */

static void leds_clear_buf(void)
{
    memset(s_gs.leds, 0, sizeof(s_gs.leds));
}

static void leds_set(int sq, rgb_t c)
{
    if (sq >= 0 && sq < 64) {
        s_gs.leds[SQ_LED(sq)] = c;
    }
}


static void rebuild_leds(void)
{
    leds_clear_buf();

    if (s_gs.phase == GP_GAME_OVER) {
        for (int i = 0; i < 64; i++) {
            s_gs.leds[i] = (i % 2 == 0) ?
                (rgb_t){30,0,0} : (rgb_t){0,0,30};
        }
        return;
    }

    if (s_gs.phase != GP_PLAYING) return;

    if (!MOVE_IS_NULL(s_gs.sf_move) && s_gs.sf_status == SF_STATUS_HAS_MOVE) {
        leds_set(s_gs.sf_move.from, COL_SF_FROM);
        leds_set(s_gs.sf_move.to,   COL_SF_TO);
    }

    /* 2. Pending move awaiting confirmation */
    if (s_gs.mds == MDS_AWAITING_CONFIRM && !MOVE_IS_NULL(s_gs.pending_move)) {
        leds_set(s_gs.pending_move.from, COL_CONFIRM_F);
        leds_set(s_gs.pending_move.to,   COL_CONFIRM_T);
    }

    /* 3. Piece-lifted: show valid destinations */
    if (s_gs.mds == MDS_PIECE_LIFTED ||
        s_gs.mds == MDS_CAPTURE_PROGRESS) {
        uint64_t dests = rules_dest_mask(&s_gs.board, s_gs.lifted_sq);
        for (int sq = 0; sq < 64; sq++) {
            if (dests & (1ULL << sq)) {
                leds_set(sq, COL_VALID_DST);
            }
        }
        leds_set(s_gs.lifted_sq, COL_MOVE_FROM);
    }

    /* 4. Check — king square pulses (overrides everything) */
    if (board_in_check(&s_gs.board, (piece_color_t)s_gs.current_player)) {
        int ksq = board_king_sq(&s_gs.board,
                                 (piece_color_t)s_gs.current_player);
        if (ksq != SQ_NONE) leds_set(ksq, COL_CHECK);
    }
}

/* ============================================================
 * MOVE INFERENCE
 * When sensor shows a capture (piece gone, destination unchanged),
 * infer destination from game logic.
 * Returns SQ_NONE if ambiguous.
 * ============================================================ */
static int infer_capture_dest(int from_sq)
{
    uint64_t candidates = rules_capture_candidates(&s_gs.board, from_sq,
                                                    s_gs.sensor_occupied);
    if (popcount64(candidates) == 1) {
        return __builtin_ctzll(candidates);
    }
    return SQ_NONE;   /* Ambiguous */
}

/* ============================================================
 * APPLY CONFIRMED MOVE
 * ============================================================ */
static void apply_confirmed_move(move_t m)
{
    /* Auto-promote to queen (physical board: no promotion UI) */
    if (m.promo == PT_NONE) {
        piece_t mover = s_gs.board.pieces[m.from];
        int promo_rank = (mover.color == PC_WHITE) ? 7 : 0;
        if (mover.type == PT_PAWN && SQ_RANK(m.to) == promo_rank) {
            m.promo = PT_QUEEN;
            ESP_LOGI(TAG, "Auto-promoting pawn to Queen");
        }
    }

    /* Record in UCI history */
    if (s_gs.history.count < MAX_HALF_MOVES) {
        move_to_uci(m, s_gs.history.entries[s_gs.history.count]);
        s_gs.history.count++;
    }

    board_apply_move(&s_gs.board, m);
    s_gs.expected_occupied = board_expected(&s_gs.board);
    s_gs.current_player    = (int)s_gs.board.active;

    /* Reset move detection */
    s_gs.mds         = MDS_IDLE;
    s_gs.lifted_sq   = SQ_NONE;
    s_gs.enemy_lift_sq = SQ_NONE;
    s_gs.pending_move  = MOVE_NULL;

    /* Invalidate old Stockfish suggestion */
    s_gs.sf_move   = MOVE_NULL;
    s_gs.sf_status = SF_STATUS_IDLE;

    ESP_LOGI(TAG, "Move applied: %s%s (move %d, %s to move)",
             s_gs.history.entries[s_gs.history.count - 1],
             m.promo != PT_NONE ? "(Q)" : "",
             s_gs.board.fullmove,
             s_gs.board.active == PC_WHITE ? "WHITE" : "BLACK");

    rebuild_leds();

    /* Check for game-over conditions */
    if (rules_is_checkmate(&s_gs.board)) {
        s_gs.phase       = GP_GAME_OVER;
        s_gs.over_reason = GO_CHECKMATE;
        s_gs.clock_running = false;
        ESP_LOGI(TAG, "CHECKMATE");
    } else if (rules_is_stalemate(&s_gs.board)) {
        s_gs.phase       = GP_GAME_OVER;
        s_gs.over_reason = GO_STALEMATE;
        s_gs.clock_running = false;
        ESP_LOGI(TAG, "STALEMATE");
    } else if (rules_is_50move(&s_gs.board)) {
        s_gs.phase       = GP_GAME_OVER;
        s_gs.over_reason = GO_DRAW_50MOVE;
        s_gs.clock_running = false;
        ESP_LOGI(TAG, "DRAW (50-move rule)");
    } else if (rules_is_insufficient_material(&s_gs.board)) {
        s_gs.phase       = GP_GAME_OVER;
        s_gs.over_reason = GO_DRAW_MATERIAL;
        s_gs.clock_running = false;
        ESP_LOGI(TAG, "DRAW (insufficient material)");
    }
}

/* ============================================================
 * SENSOR CHANGE PROCESSING
 * Called from game_logic_task when a stable sensor change is detected.
 * ============================================================ */
static void process_sensor_change(uint64_t new_state)
{
    if (s_gs.phase != GP_PLAYING) return;

    uint64_t expected = s_gs.expected_occupied;
    uint64_t removed  = sq_removed(expected, new_state);
    uint64_t added    = sq_added  (expected, new_state);

    switch (s_gs.mds) {

    /* ── IDLE: look for a piece being lifted ─────────────────── */
    case MDS_IDLE: {
        if (removed == 0) break;  /* Nothing changed (spurious read) */

        /* Find the first removed square that belongs to current player */
        uint64_t tmp = removed;
        while (tmp) {
            int sq = __builtin_ctzll(tmp);
            tmp &= tmp - 1;
            piece_t pc = s_gs.board.pieces[sq];
            if (pc.type != PT_NONE && pc.color == (piece_color_t)s_gs.current_player) {
                s_gs.lifted_sq = (int8_t)sq;
                s_gs.mds       = MDS_PIECE_LIFTED;
                ESP_LOGD(TAG, "Piece lifted from %c%d",
                         'A' + SQ_FILE(sq), 1 + SQ_RANK(sq));
                rebuild_leds();
                return;
            }
        }
        /* If it's opponent's piece removed (player lifting capture piece first) */
        if (added == 0) {
            /* Ignore — might be accidental nudge */
        }
        break;
    }

    /* ── PIECE_LIFTED: waiting for placement ─────────────────── */
    case MDS_PIECE_LIFTED: {
        int from = s_gs.lifted_sq;

        /* Player put piece back — undo */
        if (new_state == expected) {
            s_gs.mds       = MDS_IDLE;
            s_gs.lifted_sq = SQ_NONE;
            rebuild_leds();
            break;
        }

        /* Normal move: a new square appeared */
        if (popcount64(added) == 1 && popcount64(removed) == 1) {
            int to = __builtin_ctzll(added);
            if (rules_is_legal(&s_gs.board, from, to, PT_NONE)) {
                s_gs.pending_move = (move_t){ (int8_t)from, (int8_t)to, PT_NONE };
                s_gs.mds          = MDS_AWAITING_CONFIRM;
                rebuild_leds();
            } else {
                /* Illegal move — flash red on destination */
                leds_set(to, COL_INVALID);
                ESP_LOGD(TAG, "Illegal move attempt to %c%d",
                         'A' + SQ_FILE(to), 1 + SQ_RANK(to));
            }
            break;
        }

        /* Castling: 2 pieces moved */
        if (popcount64(added) == 2 && popcount64(removed) == 2) {
            /* King must be one of the removed squares */
            uint64_t r2 = removed;
            int sq1 = __builtin_ctzll(r2); r2 &= r2-1;
            int sq2 = __builtin_ctzll(r2);
            int king_sq = SQ_NONE;
            if (s_gs.board.pieces[sq1].type == PT_KING) { king_sq = sq1; }
            else if (s_gs.board.pieces[sq2].type == PT_KING) { king_sq = sq2; }
            if (king_sq != SQ_NONE) {
                /* Determine castling direction from added squares */
                uint64_t a2 = added;
                int a1 = __builtin_ctzll(a2); a2 &= a2-1;
                int a3 = __builtin_ctzll(a2);
                /* King lands on g-file (kingside) or c-file (queenside) */
                int king_dest = (SQ_FILE(a1) == 6 || SQ_FILE(a1) == 2) ? a1 : a3;
                if (rules_is_legal(&s_gs.board, king_sq, king_dest, PT_NONE)) {
                    s_gs.pending_move = (move_t){(int8_t)king_sq,(int8_t)king_dest,PT_NONE};
                    s_gs.mds          = MDS_AWAITING_CONFIRM;
                    rebuild_leds();
                }
            }
            break;
        }

        /* En passant: capturing pawn moved + captured pawn removed */
        if (popcount64(added) == 1 && popcount64(removed) == 2) {
            int to_sq = __builtin_ctzll(added);
            if (to_sq == s_gs.board.en_passant) {
                if (rules_is_legal(&s_gs.board, from, to_sq, PT_NONE)) {
                    s_gs.pending_move = (move_t){(int8_t)from,(int8_t)to_sq,PT_NONE};
                    s_gs.mds          = MDS_AWAITING_CONFIRM;
                    rebuild_leds();
                }
            }
            break;
        }

        /* Capture: piece gone, enemy piece also lifted (player removes enemy first) */
        if (popcount64(removed) == 2 && popcount64(added) == 0) {
            uint64_t r2 = removed;
            int sq1 = __builtin_ctzll(r2); r2 &= r2-1;
            int sq2 = __builtin_ctzll(r2);
            /* Identify which is ours and which is enemy */
            bool sq1_ours = (s_gs.board.pieces[sq1].color == (piece_color_t)s_gs.current_player);
            int our_sq    = sq1_ours ? sq1 : sq2;
            int enemy_sq  = sq1_ours ? sq2 : sq1;
            if (s_gs.board.pieces[enemy_sq].type != PT_NONE) {
                s_gs.lifted_sq    = (int8_t)our_sq;
                s_gs.enemy_lift_sq = (int8_t)enemy_sq;
                s_gs.mds          = MDS_CAPTURE_PROGRESS;
                rebuild_leds();
            }
            break;
        }
        break;
    }

    /* ── CAPTURE_IN_PROGRESS: our piece in hand, enemy removed ── */
    case MDS_CAPTURE_PROGRESS: {
        int from     = s_gs.lifted_sq;
        int enemy_sq = s_gs.enemy_lift_sq;

        /* Our piece placed on the enemy's square */
        if ((new_state & (1ULL << enemy_sq)) &&
            !(new_state & (1ULL << from)) &&
            rules_is_legal(&s_gs.board, from, enemy_sq, PT_NONE)) {
            s_gs.pending_move = (move_t){(int8_t)from,(int8_t)enemy_sq,PT_NONE};
            s_gs.mds          = MDS_AWAITING_CONFIRM;
            rebuild_leds();
        }
        break;
    }

    /* ── AWAITING_CONFIRM: wait for clock button — handled in button handler ─ */
    case MDS_AWAITING_CONFIRM:
        /* If board goes back to expected, cancel pending move */
        if (new_state == expected) {
            s_gs.mds          = MDS_IDLE;
            s_gs.pending_move = MOVE_NULL;
            rebuild_leds();
        }
        break;
    }
}

/* ============================================================
 * BUTTON EVENT PROCESSING
 * ============================================================ */
static void process_button(button_event_t ev)
{
    /* Sleep/wake — handled in any phase */
    if (ev == BTN_BOTH_HOLD) {
        ESP_LOGI(TAG, "Both buttons held — entering deep sleep");
        /* Save state */
        rtc_magic       = RTC_MAGIC;
        rtc_sf_side     = s_gs.sf_side;
        rtc_clock_ms[0] = s_gs.clock_ms[0];
        rtc_clock_ms[1] = s_gs.clock_ms[1];
        rtc_active_color = (uint8_t)s_gs.board.active;
        rtc_move_count   = s_gs.history.count;
        memcpy(rtc_moves, s_gs.history.entries,
               s_gs.history.count * sizeof(rtc_moves[0]));
        board_to_fen(&s_gs.board, rtc_fen, sizeof(rtc_fen));

        display_message("SLEEP", "Hold both to wake");
        leds_clear();

        /* Wake on either button (active-LOW → wake on LOW) */
        esp_sleep_enable_ext1_wakeup(
            (1ULL << PIN_BTN_P1) | (1ULL << PIN_BTN_P2),
            ESP_EXT1_WAKEUP_ANY_LOW);
        esp_deep_sleep_start();
        /* Does not return */
    }

    if (s_gs.phase == GP_SELECT_SIDE) {
        if (ev == BTN_P1_SHORT) {
            s_gs.sf_side = SF_SIDE_WHITE;
            ESP_LOGI(TAG, "Stockfish assists WHITE");
            s_gs.phase   = GP_PLAYING;
            s_gs.clock_running = true;
        } else if (ev == BTN_P2_SHORT) {
            s_gs.sf_side = SF_SIDE_BLACK;
            ESP_LOGI(TAG, "Stockfish assists BLACK");
            s_gs.phase   = GP_PLAYING;
            s_gs.clock_running = true;
        }
        return;
    }

    if (s_gs.phase != GP_PLAYING) return;

    /* Clock button pressed: confirm move and switch turn */
    bool is_p1 = (ev == BTN_P1_SHORT);
    bool is_p2 = (ev == BTN_P2_SHORT);
    bool correct_player = (is_p1 && s_gs.current_player == 0) ||
                          (is_p2 && s_gs.current_player == 1);

    if (!correct_player) return;  /* Wrong player pressed clock */

    /* ── Confirm the pending move ─────────────────────────────── */
    if (s_gs.mds == MDS_AWAITING_CONFIRM && !MOVE_IS_NULL(s_gs.pending_move)) {
        /* Stop current player's clock */
        s_gs.clock_running = false;  /* Temporarily pause */

        apply_confirmed_move(s_gs.pending_move);
        if (s_gs.phase == GP_GAME_OVER) return;

        /* Start new active player's clock */
        s_gs.clock_running = true;

        /* Trigger Stockfish if it assists the new active player */
        if ((s_gs.current_player == 0 && s_gs.sf_side == SF_SIDE_WHITE) ||
            (s_gs.current_player == 1 && s_gs.sf_side == SF_SIDE_BLACK)) {
            s_gs.sf_status = SF_STATUS_THINKING;
            /* Stockfish request is picked up by game_logic_task */
        }
        return;
    }

    /* ── Capture inference: player pressed clock while piece still in hand ─ */
    if (s_gs.mds == MDS_PIECE_LIFTED) {
        int cap_dest = infer_capture_dest(s_gs.lifted_sq);
        if (cap_dest != SQ_NONE) {
            s_gs.pending_move = (move_t){(int8_t)s_gs.lifted_sq,
                                         (int8_t)cap_dest, PT_NONE};
            s_gs.mds          = MDS_AWAITING_CONFIRM;
            /* Re-process as confirmation */
            process_button(ev);
        } else {
            ESP_LOGW(TAG, "Ambiguous capture — cannot confirm");
            /* Flash invalid colour on all candidate squares */
            uint64_t cands = rules_capture_candidates(&s_gs.board,
                                                       s_gs.lifted_sq,
                                                       s_gs.sensor_occupied);
            for (int sq = 0; sq < 64; sq++) {
                if (cands & (1ULL << sq)) leds_set(sq, COL_INVALID);
            }
        }
    }
}

/* ============================================================
 * SLEEP WAKE RESTORE
 * ============================================================ */
static bool restore_from_sleep(void)
{
    if (rtc_magic != RTC_MAGIC) return false;

    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    if (cause != ESP_SLEEP_WAKEUP_EXT1) return false;

    /* Wait up to 5 s with both buttons held to confirm wake */
    display_message("WAKE?", "Hold both 5s...");
    uint32_t start = now_ms();
    while (now_ms() - start < SLEEP_HOLD_BOTH_MS) {
        bool p1 = (gpio_get_level(PIN_BTN_P1) == 0);
        bool p2 = (gpio_get_level(PIN_BTN_P2) == 0);
        if (!p1 || !p2) {
            /* Released too early — back to sleep */
            ESP_LOGI(TAG, "Early release — back to sleep");
            esp_deep_sleep_start();
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    /* Full 5-second hold confirmed — restore game state */
    ESP_LOGI(TAG, "Restoring game from RTC memory");

    s_gs.sf_side      = rtc_sf_side;
    s_gs.clock_ms[0]  = rtc_clock_ms[0];
    s_gs.clock_ms[1]  = rtc_clock_ms[1];
    s_gs.board.active = (piece_color_t)rtc_active_color;
    memcpy(s_gs.history.entries, rtc_moves,
           rtc_move_count * sizeof(rtc_moves[0]));
    s_gs.history.count   = rtc_move_count;
    s_gs.current_player  = (int)rtc_active_color;
    s_gs.expected_occupied = board_expected(&s_gs.board);
    s_gs.phase           = GP_PLAYING;
    s_gs.clock_running   = true;

    rtc_magic = 0;  /* Clear so next boot doesn't restore stale state */
    return true;
}

/* ============================================================
 * SENSOR TASK  (Core 0)
 * Polls sensors every BOARD_POLL_MS ms and notifies game_logic_task
 * when a stable change is detected.
 * ============================================================ */
void game_sensor_task(void *arg)
{
    (void)arg;
    uint64_t prev_reading = 0;
    uint64_t stable_reading = 0;
    uint32_t stable_since = 0;
    bool     stable_pending = false;

    /* Initial read */
    sensors_read(&prev_reading);
    stable_reading = prev_reading;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(BOARD_POLL_MS));

        uint64_t current;
        sensors_read(&current);

        /* Always refresh APA102C after HC165 read (shared CLK) */
        gs_lock();
        leds_write(s_gs.leds);  /* Re-send last LED state */
        gs_unlock();

        uint32_t t = now_ms();

        if (current != prev_reading) {
            /* Board changed — reset stability timer */
            prev_reading  = current;
            stable_since  = t;
            stable_pending = true;
        }

        if (stable_pending && (t - stable_since) >= BOARD_STABLE_MS) {
            /* Board has been stable for BOARD_STABLE_MS — process change */
            stable_pending = false;
            if (current != stable_reading) {
                stable_reading = current;
                gs_lock();
                s_gs.sensor_occupied = current;
                process_sensor_change(current);
                gs_unlock();
            }
        }
    }
}

/* ============================================================
 * GAME LOGIC TASK  (Core 0)
 * Handles button events, clock ticking, and Stockfish requests.
 * ============================================================ */
void game_logic_task(void *arg)
{
    (void)arg;
    uint32_t last_clock_tick = now_ms();

    for (;;) {
        /* ── Process button events ──────────────────────────────── */
        button_event_t ev;
        while (xQueueReceive(g_button_queue, &ev, 0) == pdTRUE) {
            gs_lock();
            process_button(ev);
            gs_unlock();
        }

        /* ── Clock tick ─────────────────────────────────────────── */
        uint32_t t = now_ms();
        if ((t - last_clock_tick) >= CLOCK_TICK_MS) {
            gs_lock();
            if (s_gs.phase == GP_PLAYING && s_gs.clock_running) {
                uint32_t elapsed = t - last_clock_tick;
                int p = s_gs.current_player;
                s_gs.clock_ms[p] -= (int32_t)elapsed;
                if (s_gs.clock_ms[p] <= 0) {
                    s_gs.clock_ms[p] = 0;
                    s_gs.phase       = GP_GAME_OVER;
                    s_gs.over_reason = (p == 0) ? GO_TIME_WHITE : GO_TIME_BLACK;
                    s_gs.clock_running = false;
                    ESP_LOGI(TAG, "Player %d ran out of time", p);
                }
            }
            last_clock_tick = t;
            gs_unlock();
        }

        /* ── Starting position check ────────────────────────────── */
        gs_lock();
        if (s_gs.phase == GP_WAIT_POSITION) {
            uint64_t expected = sensors_start_mask();
            if (s_gs.sensor_occupied == expected) {
                /* All 32 pieces in starting position — ask for side selection */
                s_gs.phase = GP_SELECT_SIDE;
                board_init_start(&s_gs.board);
                s_gs.expected_occupied = board_expected(&s_gs.board);
                s_gs.history.count     = 0;
                s_gs.clock_ms[0]       = GAME_CLOCK_MS;
                s_gs.clock_ms[1]       = GAME_CLOCK_MS;
                s_gs.current_player    = 0;  /* White moves first */
                s_gs.mds               = MDS_IDLE;
                s_gs.sf_move           = MOVE_NULL;
                ESP_LOGI(TAG, "Starting position confirmed");
                leds_clear_buf();
            }
        }
        gs_unlock();

        /* ── Stockfish analysis request ─────────────────────────── */
        gs_lock();
        bool need_sf = (s_gs.phase == GP_PLAYING &&
                        s_gs.sf_status == SF_STATUS_THINKING &&
                        uci_ready());
        gs_unlock();

        if (need_sf) {
            gs_lock();
            move_history_t hist_copy = s_gs.history;
            gs_unlock();

            uci_send_position(&hist_copy);
            uci_go_depth(SF_SEARCH_DEPTH);

            move_t best = MOVE_NULL;
            int depth = 0, score = 0;
            bool ok = uci_wait_bestmove(&best, &depth, &score, 0);

            gs_lock();
            if (ok && !MOVE_IS_NULL(best)) {
                s_gs.sf_move   = best;
                s_gs.sf_status = SF_STATUS_HAS_MOVE;
                rebuild_leds();
                char mv[6];
                move_to_uci(best, mv);
                ESP_LOGI(TAG, "SF bestmove: %s (depth %d, score %+d cp)",
                         mv, depth, score);
            } else {
                s_gs.sf_status = SF_STATUS_IDLE;
            }
            gs_unlock();
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

/* ============================================================
 * LED TASK  (Core 0)
 * Writes LED buffer to hardware whenever it changes.
 * The sensor task calls leds_write() post-read; this task handles
 * any game-state-triggered changes between sensor polls.
 * ============================================================ */
void game_led_task(void *arg)
{
    (void)arg;
    rgb_t last_sent[64];
    memset(last_sent, 0, sizeof(last_sent));

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(50));   /* 20 Hz refresh */
        gs_lock();
        bool changed = (memcmp(last_sent, s_gs.leds, sizeof(s_gs.leds)) != 0);
        rgb_t snapshot[64];
        memcpy(snapshot, s_gs.leds, sizeof(s_gs.leds));
        gs_unlock();

        if (changed) {
            leds_write(snapshot);
            memcpy(last_sent, snapshot, sizeof(snapshot));
        }
    }
}

/* ============================================================
 * DISPLAY TASK  (Core 0)
 * ============================================================ */
void game_display_task(void *arg)
{
    (void)arg;
    char msg1[32];

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(200));  /* 5 Hz */

        gs_lock();
        game_phase_t phase    = s_gs.phase;
        int32_t  clk_w        = s_gs.clock_ms[0];
        int32_t  clk_b        = s_gs.clock_ms[1];
        int      active       = s_gs.current_player;
        int      move_num     = s_gs.board.fullmove;
        uint8_t  bat          = s_gs.battery_pct;
        bool     thinking     = (s_gs.sf_status == SF_STATUS_THINKING);
        game_over_reason_t over = s_gs.over_reason;
        gs_unlock();

        switch (phase) {
        case GP_BOOT:
            display_message("CHESS BOARD", "Initialising...");
            break;
        case GP_WAIT_POSITION:
            display_message("Place pieces in", "starting position");
            break;
        case GP_SELECT_SIDE:
            display_message("P1=White  P2=Black", "Stockfish assists?");
            break;
        case GP_PLAYING:
            display_update(clk_w, clk_b, active, move_num,
                           (int)bat, thinking, 0, 0);
            break;
        case GP_GAME_OVER:
            switch (over) {
            case GO_CHECKMATE:
                snprintf(msg1, sizeof(msg1), "%s wins!",
                         active == 0 ? "BLACK" : "WHITE");
                display_message(msg1, "Checkmate");
                break;
            case GO_STALEMATE:
                display_message("Draw", "Stalemate");
                break;
            case GO_TIME_WHITE:
                display_message("BLACK wins!", "White on time");
                break;
            case GO_TIME_BLACK:
                display_message("WHITE wins!", "Black on time");
                break;
            case GO_DRAW_50MOVE:
                display_message("Draw", "50-move rule");
                break;
            case GO_DRAW_MATERIAL:
                display_message("Draw", "Insufficient material");
                break;
            default:
                display_message("Game Over", "");
                break;
            }
            break;
        case GP_SLEEPING:
            display_message("SLEEP", "Hold both to wake");
            break;
        default: break;
        }
    }
}

/* ============================================================
 * GAME INIT
 * ============================================================ */

esp_err_t game_init(void)
{
    /* Mutex for shared game state */
    s_mutex = xSemaphoreCreateMutex();
    if (!s_mutex) return ESP_ERR_NO_MEM;

    /* Button event queue */
    g_button_queue = xQueueCreate(16, sizeof(button_event_t));
    if (!g_button_queue) return ESP_ERR_NO_MEM;

    /* Zero game state */
    memset(&s_gs, 0, sizeof(s_gs));
    s_gs.lifted_sq    = SQ_NONE;
    s_gs.enemy_lift_sq = SQ_NONE;
    s_gs.pending_move = MOVE_NULL;
    s_gs.sf_move      = MOVE_NULL;
    s_gs.battery_pct  = 100;

    /* Attempt wake-from-sleep restore */
    if (restore_from_sleep()) {
        ESP_LOGI(TAG, "Resumed from deep sleep");
    } else {
        s_gs.phase = GP_BOOT;
        board_init_start(&s_gs.board);
        s_gs.clock_ms[0] = GAME_CLOCK_MS;
        s_gs.clock_ms[1] = GAME_CLOCK_MS;
    }

    /* Precompute chess move tables */
    rules_init();

    /* ── Create tasks ────────────────────────────────────────── */
    xTaskCreatePinnedToCore(game_sensor_task,  "sensor",
                            TASK_STACK_SENSOR,  NULL,
                            TASK_PRIO_SENSOR,   NULL, CORE_GAME);

    xTaskCreatePinnedToCore(game_logic_task,   "game",
                            TASK_STACK_GAME,    NULL,
                            TASK_PRIO_GAME,     NULL, CORE_GAME);

    xTaskCreatePinnedToCore(game_led_task,     "leds",
                            TASK_STACK_LED,     NULL,
                            TASK_PRIO_LED,      NULL, CORE_GAME);

    xTaskCreatePinnedToCore(game_display_task, "display",
                            TASK_STACK_DISPLAY, NULL,
                            TASK_PRIO_DISPLAY,  NULL, CORE_GAME);

    xTaskCreatePinnedToCore(buttons_task,       "buttons",
                            TASK_STACK_BUTTON,  &g_button_queue,
                            TASK_PRIO_BUTTON,   NULL, CORE_GAME);

    xTaskCreatePinnedToCore(battery_task,       "battery",
                            TASK_STACK_BATTERY, NULL,
                            TASK_PRIO_BATTERY,  NULL, CORE_GAME);

    /* Transition out of boot */
    gs_lock();
    if (s_gs.phase == GP_BOOT) {
        s_gs.phase = GP_WAIT_POSITION;
    }
    gs_unlock();

    ESP_LOGI(TAG, "Game init complete");
    return ESP_OK;
}
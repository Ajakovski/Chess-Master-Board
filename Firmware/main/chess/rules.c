#include "rules.h"
#include "board.h"
#include "config.h"
#include <string.h>

/* ============================================================
 * PRECOMPUTED ATTACK TABLES
 * ============================================================ */
static uint64_t s_knight_attacks[64];
static uint64_t s_king_attacks[64];
static bool     s_tables_ready = false;

void rules_init(void)
{
    if (s_tables_ready) return;

    static const int kdx[8] = { 1, 2, 2, 1,-1,-2,-2,-1};
    static const int kdy[8] = { 2, 1,-1,-2,-2,-1, 1, 2};
    static const int gx[8]  = { 1, 0,-1, 0, 1,-1, 1,-1};
    static const int gy[8]  = { 0, 1, 0,-1, 1, 1,-1,-1};

    for (int sq = 0; sq < 64; sq++) {
        int f = SQ_FILE(sq), r = SQ_RANK(sq);
        uint64_t ka = 0, ga = 0;
        for (int i = 0; i < 8; i++) {
            int nf, nr;
            nf = f + kdx[i]; nr = r + kdy[i];
            if (nf >= 0 && nf < 8 && nr >= 0 && nr < 8) ka |= 1ULL << SQ(nf,nr);
            nf = f + gx[i];  nr = r + gy[i];
            if (nf >= 0 && nf < 8 && nr >= 0 && nr < 8) ga |= 1ULL << SQ(nf,nr);
        }
        s_knight_attacks[sq] = ka;
        s_king_attacks[sq]   = ga;
    }
    s_tables_ready = true;
}

static bool leaves_in_check(const board_t *b, move_t m)
{
    board_t copy = *b;
    board_apply_move(&copy, m);
    /* After apply, active color has switched — check the side that just moved */
    piece_color_t just_moved = (copy.active == PC_WHITE) ? PC_BLACK : PC_WHITE;
    return board_in_check(&copy, just_moved);
}


static int gen_pawn(const board_t *b, int sq, move_t *out)
{
    int n = 0;
    piece_color_t col   = b->pieces[sq].color;
    int           dir   = (col == PC_WHITE) ? 1 : -1;
    int           start = (col == PC_WHITE) ? 1 : 6;  /* Starting rank */
    int           promo = (col == PC_WHITE) ? 7 : 0;  /* Promotion rank */

    int f = SQ_FILE(sq), r = SQ_RANK(sq);

    /* Forward one step */
    int r1 = r + dir;
    if (r1 >= 0 && r1 < 8) {
        int fwd = SQ(f, r1);
        if (b->pieces[fwd].type == PT_NONE) {
            if (r1 == promo) {
                /* Generate all 4 promotion choices */
                static const piece_type_t promos[4] = {
                    PT_QUEEN, PT_ROOK, PT_BISHOP, PT_KNIGHT
                };
                for (int p = 0; p < 4; p++) {
                    out[n++] = (move_t){ (int8_t)sq, (int8_t)fwd, promos[p] };
                }
            } else {
                out[n++] = (move_t){ (int8_t)sq, (int8_t)fwd, PT_NONE };
                /* Forward two from starting rank */
                if (r == start) {
                    int r2 = r + 2*dir;
                    int fwd2 = SQ(f, r2);
                    if (b->pieces[fwd2].type == PT_NONE) {
                        out[n++] = (move_t){ (int8_t)sq, (int8_t)fwd2, PT_NONE };
                    }
                }
            }
        }
        /* Diagonal captures */
        for (int df = -1; df <= 1; df += 2) {
            int cf = f + df;
            if (cf >= 0 && cf < 8) {
                int cap = SQ(cf, r1);
                bool is_enemy = (b->pieces[cap].type != PT_NONE &&
                                 b->pieces[cap].color != col);
                bool is_ep    = (cap == b->en_passant);
                if (is_enemy || is_ep) {
                    if (r1 == promo) {
                        static const piece_type_t promos[4] = {
                            PT_QUEEN, PT_ROOK, PT_BISHOP, PT_KNIGHT
                        };
                        for (int p = 0; p < 4; p++) {
                            out[n++] = (move_t){ (int8_t)sq, (int8_t)cap, promos[p] };
                        }
                    } else {
                        out[n++] = (move_t){ (int8_t)sq, (int8_t)cap, PT_NONE };
                    }
                }
            }
        }
    }
    return n;
}

static int gen_knight(const board_t *b, int sq, move_t *out)
{
    int n = 0;
    piece_color_t col = b->pieces[sq].color;
    uint64_t atk = s_knight_attacks[sq];
    while (atk) {
        int to = __builtin_ctzll(atk);
        atk &= atk - 1;
        if (b->pieces[to].type == PT_NONE || b->pieces[to].color != col) {
            out[n++] = (move_t){ (int8_t)sq, (int8_t)to, PT_NONE };
        }
    }
    return n;
}

static int gen_slider(const board_t *b, int sq, move_t *out,
                      const int *dfs, const int *drs, int ndirs)
{
    int n = 0;
    piece_color_t col = b->pieces[sq].color;
    for (int d = 0; d < ndirs; d++) {
        int f = SQ_FILE(sq) + dfs[d];
        int r = SQ_RANK(sq) + drs[d];
        while (f >= 0 && f < 8 && r >= 0 && r < 8) {
            int to = SQ(f, r);
            if (b->pieces[to].type != PT_NONE) {
                if (b->pieces[to].color != col) {
                    out[n++] = (move_t){ (int8_t)sq, (int8_t)to, PT_NONE };
                }
                break;  /* Ray blocked */
            }
            out[n++] = (move_t){ (int8_t)sq, (int8_t)to, PT_NONE };
            f += dfs[d]; r += drs[d];
        }
    }
    return n;
}

static int gen_bishop(const board_t *b, int sq, move_t *out)
{
    static const int dfs[4] = { 1,-1, 1,-1};
    static const int drs[4] = { 1, 1,-1,-1};
    return gen_slider(b, sq, out, dfs, drs, 4);
}

static int gen_rook(const board_t *b, int sq, move_t *out)
{
    static const int dfs[4] = { 1,-1, 0, 0};
    static const int drs[4] = { 0, 0, 1,-1};
    return gen_slider(b, sq, out, dfs, drs, 4);
}

static int gen_queen(const board_t *b, int sq, move_t *out)
{
    static const int dfs[8] = { 1,-1, 0, 0, 1,-1, 1,-1};
    static const int drs[8] = { 0, 0, 1,-1, 1, 1,-1,-1};
    return gen_slider(b, sq, out, dfs, drs, 8);
}

static int gen_king(const board_t *b, int sq, move_t *out)
{
    int n = 0;
    piece_color_t col   = b->pieces[sq].color;
    piece_color_t enemy = (col == PC_WHITE) ? PC_BLACK : PC_WHITE;

    /* Normal king moves */
    uint64_t atk = s_king_attacks[sq];
    while (atk) {
        int to = __builtin_ctzll(atk);
        atk &= atk - 1;
        if (b->pieces[to].type == PT_NONE || b->pieces[to].color != col) {
            out[n++] = (move_t){ (int8_t)sq, (int8_t)to, PT_NONE };
        }
    }

    /* Castling */
    int r = SQ_RANK(sq);
    if (!board_sq_attacked(b, sq, enemy)) {  /* King must not be in check */
        /* Kingside */
        uint8_t ks_right = (col == PC_WHITE) ? CASTLE_WK : CASTLE_BK;
        if ((b->castling & ks_right) &&
            b->pieces[SQ(5,r)].type == PT_NONE &&
            b->pieces[SQ(6,r)].type == PT_NONE &&
            !board_sq_attacked(b, SQ(5,r), enemy) &&
            !board_sq_attacked(b, SQ(6,r), enemy)) {
            out[n++] = (move_t){ (int8_t)sq, (int8_t)SQ(6,r), PT_NONE };
        }
        /* Queenside */
        uint8_t qs_right = (col == PC_WHITE) ? CASTLE_WQ : CASTLE_BQ;
        if ((b->castling & qs_right) &&
            b->pieces[SQ(3,r)].type == PT_NONE &&
            b->pieces[SQ(2,r)].type == PT_NONE &&
            b->pieces[SQ(1,r)].type == PT_NONE &&
            !board_sq_attacked(b, SQ(3,r), enemy) &&
            !board_sq_attacked(b, SQ(2,r), enemy)) {
            out[n++] = (move_t){ (int8_t)sq, (int8_t)SQ(2,r), PT_NONE };
        }
    }
    return n;
}

/* ============================================================
 * PUBLIC API
 * ============================================================ */

int rules_gen_moves_from(const board_t *b, int from, move_t *moves)
{
    move_t pseudo[MAX_MOVES_PER_POS];
    int np = 0;
    piece_t pc = b->pieces[from];

    if (pc.type == PT_NONE || pc.color != b->active) return 0;

    switch (pc.type) {
        case PT_PAWN:   np = gen_pawn  (b, from, pseudo); break;
        case PT_KNIGHT: np = gen_knight(b, from, pseudo); break;
        case PT_BISHOP: np = gen_bishop(b, from, pseudo); break;
        case PT_ROOK:   np = gen_rook  (b, from, pseudo); break;
        case PT_QUEEN:  np = gen_queen (b, from, pseudo); break;
        case PT_KING:   np = gen_king  (b, from, pseudo); break;
        default: break;
    }

    int nl = 0;
    for (int i = 0; i < np; i++) {
        if (!leaves_in_check(b, pseudo[i])) {
            moves[nl++] = pseudo[i];
        }
    }
    return nl;
}

int rules_gen_all_moves(const board_t *b, move_t *moves)
{
    int total = 0;
    for (int sq = 0; sq < 64; sq++) {
        if (b->pieces[sq].type  != PT_NONE &&
            b->pieces[sq].color == b->active) {
            total += rules_gen_moves_from(b, sq, moves + total);
        }
    }
    return total;
}

bool rules_is_legal(const board_t *b, int from, int to, piece_type_t promo)
{
    if (!SQ_VALID(from) || !SQ_VALID(to)) return false;
    if (b->pieces[from].type  == PT_NONE)  return false;
    if (b->pieces[from].color != b->active) return false;

    move_t pseudo[MAX_MOVES_PER_POS];
    int np = rules_gen_moves_from(b, from, pseudo);
    for (int i = 0; i < np; i++) {
        if (pseudo[i].to == to) {
            /* For promotion moves, require promo match if specified */
            if (pseudo[i].promo == PT_NONE || promo == PT_NONE ||
                pseudo[i].promo == promo) {
                return true;
            }
        }
    }
    return false;
}

uint64_t rules_dest_mask(const board_t *b, int from)
{
    move_t moves[MAX_MOVES_PER_POS];
    int n = rules_gen_moves_from(b, from, moves);
    uint64_t mask = 0;
    for (int i = 0; i < n; i++) {
        mask |= (1ULL << moves[i].to);
    }
    return mask;
}

bool rules_is_checkmate(const board_t *b)
{
    if (!board_in_check(b, b->active)) return false;
    move_t moves[MAX_MOVES_PER_POS];
    return (rules_gen_all_moves(b, moves) == 0);
}

bool rules_is_stalemate(const board_t *b)
{
    if (board_in_check(b, b->active)) return false;
    move_t moves[MAX_MOVES_PER_POS];
    return (rules_gen_all_moves(b, moves) == 0);
}

bool rules_is_insufficient_material(const board_t *b)
{
    int wn = 0, bn = 0, wb = 0, bb = 0, total = 0;
    bool wb_light = false, wb_dark = false;
    bool bb_light = false, bb_dark = false;

    for (int sq = 0; sq < 64; sq++) {
        piece_t p = b->pieces[sq];
        if (p.type == PT_NONE) continue;
        total++;
        /* Presence of pawn, rook, or queen = sufficient material */
        if (p.type == PT_PAWN || p.type == PT_ROOK || p.type == PT_QUEEN) {
            return false;
        }
        if (p.type == PT_KNIGHT) {
            if (p.color == PC_WHITE) wn++; else bn++;
        }
        if (p.type == PT_BISHOP) {
            bool light = ((SQ_FILE(sq) + SQ_RANK(sq)) % 2 == 0);
            if (p.color == PC_WHITE) { wb++; if (light) wb_light=true; else wb_dark=true; }
            else                     { bb++; if (light) bb_light=true; else bb_dark=true; }
        }
    }
    /* K vs K */
    if (total == 2) return true;
    /* K+B vs K or K+N vs K */
    if (total == 3 && (wb == 1 || wn == 1 || bb == 1 || bn == 1)) return true;
    /* K+B vs K+B (same color bishops) */
    if (total == 4 && wb == 1 && bb == 1) {
        if ((wb_light && bb_light) || (wb_dark && bb_dark)) return true;
    }
    return false;
}

uint64_t rules_capture_candidates(const board_t *b, int from,
                                   uint64_t sensor_occupied)
{
    if (!SQ_VALID(from) || b->pieces[from].type == PT_NONE) return 0;

    piece_color_t col   = b->pieces[from].color;
    piece_color_t enemy = (col == PC_WHITE) ? PC_BLACK : PC_WHITE;

    /* All squares where an enemy piece is expected AND sensor still reads occupied */
    uint64_t enemy_expected = 0;
    for (int sq = 0; sq < 64; sq++) {
        if (b->pieces[sq].type  != PT_NONE &&
            b->pieces[sq].color == enemy) {
            enemy_expected |= (1ULL << sq);
        }
    }
    uint64_t candidates = enemy_expected & sensor_occupied;

    /* Filter to reachable attack squares for this piece */
    move_t moves[MAX_MOVES_PER_POS];
    int n = rules_gen_moves_from(b, from, moves);
    uint64_t reach = 0;
    for (int i = 0; i < n; i++) reach |= (1ULL << moves[i].to);

    return candidates & reach;
}
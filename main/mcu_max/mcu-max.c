#include "mcu-max.h"
#include "config.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* ── UCI bridge hooks — explicit forward declarations (defined in uci.c) */
extern void uci_engine_getline(char *buf, size_t sz);
extern void uci_engine_puts(const char *line);

/* ============================================================
 * CONSTANTS
 * ============================================================ */

/*
 * config.h defines SQ/SQ_FILE/SQ_RANK for the 8×8 game board.
 * mcu-max uses a 0x88 board with different formulas.
 * Undefine config.h versions before defining our own.
 */
#ifdef SQ
#undef SQ
#endif
#ifdef SQ_FILE
#undef SQ_FILE
#endif
#ifdef SQ_RANK
#undef SQ_RANK
#endif

/* 0x88 square helpers */
#define SQ(f,r)         (uint8_t)(((r) << 4) | (f))
#define SQ_FILE(s)      ((s) & 7)
#define SQ_RANK(s)      ((s) >> 4)
#define ON_BOARD(s)     (!((s) & 0x88))

/* Named squares */
#define A1  SQ(0,0)
#define E1  SQ(4,0)
#define H1  SQ(7,0)
#define A8  SQ(0,7)
#define E8  SQ(4,7)
#define H8  SQ(7,7)

/* Piece type/color */
#define EMPTY   0
#define WHITE   0
#define BLACK   1

#define WP 1
#define WN 2
#define WB 3
#define WR 4
#define WQ 5
#define WK 6
#define BP 9
#define BN 10
#define BB 11
#define BRK 12  /* Black Rook — BR conflicts with Xtensa specreg.h */
#define BQ 13
#define BK 14

#define PIECE_TYPE(p)   ((uint8_t)(p) & 7)
#define PIECE_COLOR(p)  ((uint8_t)(p) >> 3)
#define MAKE_PIECE(t,c) ((uint8_t)((t) | ((c) << 3)))
#define IS_PIECE(p)     ((uint8_t)(p) != EMPTY)
#define SAME_COLOR(a,b) (((uint8_t)(a) != EMPTY) && \
                         ((uint8_t)(b) != EMPTY) && \
                         (PIECE_COLOR(a) == PIECE_COLOR(b)))

/* Castling rights bitmask */
#define CWK  1   /* White kingside  */
#define CWQ  2   /* White queenside */
#define CBK  4   /* Black kingside  */
#define CBQ  8   /* Black queenside */

/* Search */
#define MAX_PLY         20
#define INF             30000
#define MATE_BASE       29000   /* MATE_BASE - ply = score for mate in ply moves */
#define IS_MATE(s)      (abs(s) > 28000)

/* Hash table entry types */
#define HT_EXACT  0
#define HT_LOWER  1
#define HT_UPPER  2

/* Null square */
#define NO_SQ   0xFF

/* ============================================================
 * PIECE VALUES AND TABLES
 * ============================================================ */

/* Centipawn material values indexed by piece code 0-14 */
static const int16_t PIECE_VAL[16] = {
    0,   /* 0  empty  */
    100, /* 1  WP     */
    320, /* 2  WN     */
    330, /* 3  WB     */
    500, /* 4  WR     */
    900, /* 5  WQ     */
  20000, /* 6  WK     */
    0,   /* 7  unused */
    0,   /* 8  unused */
    100, /* 9  BP     */
    320, /* 10 BN     */
    330, /* 11 BB     */
    500, /* 12 BRK    */
    900, /* 13 BQ     */
  20000, /* 14 BK     */
    0,   /* 15 unused */
};

/*
 * Piece-square bonus table for white (positive = good for white).
 * Indexed by sq (0x88 format), so only valid squares (& ~0x88) are used.
 * Values in centipawns.  Mirrors are applied for black pieces.
 *
 * Layout is rank 0 (bottom) to rank 7 (top), left to right.
 * Stored in a flat 128-element array matching the 0x88 board.
 */
static const int8_t PST_PAWN[128] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,0,0,0,0,0,0,0,
     5, 10, 10,-20,-20, 10, 10,  5,  0,0,0,0,0,0,0,0,
     5, -5,-10,  0,  0,-10, -5,  5,  0,0,0,0,0,0,0,0,
     0,  0,  0, 20, 20,  0,  0,  0,  0,0,0,0,0,0,0,0,
     5,  5, 10, 25, 25, 10,  5,  5,  0,0,0,0,0,0,0,0,
    10, 10, 20, 30, 30, 20, 10, 10,  0,0,0,0,0,0,0,0,
    50, 50, 50, 50, 50, 50, 50, 50,  0,0,0,0,0,0,0,0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,0,0,0,0,0,0,0,
};

static const int8_t PST_KNIGHT[128] = {
    -50,-40,-30,-30,-30,-30,-40,-50, 0,0,0,0,0,0,0,0,
    -40,-20,  0,  5,  5,  0,-20,-40, 0,0,0,0,0,0,0,0,
    -30,  5, 10, 15, 15, 10,  5,-30, 0,0,0,0,0,0,0,0,
    -30,  0, 15, 20, 20, 15,  0,-30, 0,0,0,0,0,0,0,0,
    -30,  5, 15, 20, 20, 15,  5,-30, 0,0,0,0,0,0,0,0,
    -30,  0, 10, 15, 15, 10,  0,-30, 0,0,0,0,0,0,0,0,
    -40,-20,  0,  0,  0,  0,-20,-40, 0,0,0,0,0,0,0,0,
    -50,-40,-30,-30,-30,-30,-40,-50, 0,0,0,0,0,0,0,0,
};

static const int8_t PST_BISHOP[128] = {
    -20,-10,-10,-10,-10,-10,-10,-20, 0,0,0,0,0,0,0,0,
    -10,  5,  0,  0,  0,  0,  5,-10, 0,0,0,0,0,0,0,0,
    -10, 10, 10, 10, 10, 10, 10,-10, 0,0,0,0,0,0,0,0,
    -10,  0, 10, 10, 10, 10,  0,-10, 0,0,0,0,0,0,0,0,
    -10,  5,  5, 10, 10,  5,  5,-10, 0,0,0,0,0,0,0,0,
    -10,  0,  5, 10, 10,  5,  0,-10, 0,0,0,0,0,0,0,0,
    -10,  0,  0,  0,  0,  0,  0,-10, 0,0,0,0,0,0,0,0,
    -20,-10,-10,-10,-10,-10,-10,-20, 0,0,0,0,0,0,0,0,
};

static const int8_t PST_ROOK[128] = {
     0,  0,  0,  5,  5,  0,  0,  0, 0,0,0,0,0,0,0,0,
    -5,  0,  0,  0,  0,  0,  0, -5, 0,0,0,0,0,0,0,0,
    -5,  0,  0,  0,  0,  0,  0, -5, 0,0,0,0,0,0,0,0,
    -5,  0,  0,  0,  0,  0,  0, -5, 0,0,0,0,0,0,0,0,
    -5,  0,  0,  0,  0,  0,  0, -5, 0,0,0,0,0,0,0,0,
    -5,  0,  0,  0,  0,  0,  0, -5, 0,0,0,0,0,0,0,0,
     5, 10, 10, 10, 10, 10, 10,  5, 0,0,0,0,0,0,0,0,
     0,  0,  0,  0,  0,  0,  0,  0, 0,0,0,0,0,0,0,0,
};

static const int8_t PST_QUEEN[128] = {
    -20,-10,-10, -5, -5,-10,-10,-20, 0,0,0,0,0,0,0,0,
    -10,  0,  5,  0,  0,  0,  0,-10, 0,0,0,0,0,0,0,0,
    -10,  5,  5,  5,  5,  5,  0,-10, 0,0,0,0,0,0,0,0,
      0,  0,  5,  5,  5,  5,  0, -5, 0,0,0,0,0,0,0,0,
     -5,  0,  5,  5,  5,  5,  0, -5, 0,0,0,0,0,0,0,0,
    -10,  0,  5,  5,  5,  5,  0,-10, 0,0,0,0,0,0,0,0,
    -10,  0,  0,  0,  0,  0,  0,-10, 0,0,0,0,0,0,0,0,
    -20,-10,-10, -5, -5,-10,-10,-20, 0,0,0,0,0,0,0,0,
};

static const int8_t PST_KING_MG[128] = {
     20, 30, 10,  0,  0, 10, 30, 20, 0,0,0,0,0,0,0,0,
     20, 20,  0,  0,  0,  0, 20, 20, 0,0,0,0,0,0,0,0,
    -10,-20,-20,-20,-20,-20,-20,-10, 0,0,0,0,0,0,0,0,
    -20,-30,-30,-40,-40,-30,-30,-20, 0,0,0,0,0,0,0,0,
    -30,-40,-40,-50,-50,-40,-40,-30, 0,0,0,0,0,0,0,0,
    -30,-40,-40,-50,-50,-40,-40,-30, 0,0,0,0,0,0,0,0,
    -30,-40,-40,-50,-50,-40,-40,-30, 0,0,0,0,0,0,0,0,
    -30,-40,-40,-50,-50,-40,-40,-30, 0,0,0,0,0,0,0,0,
};

/* Mirror a PST index for black (flip rank) */
static inline uint8_t mirror_sq(uint8_t sq)
{
    return (uint8_t)(((7 - SQ_RANK(sq)) << 4) | SQ_FILE(sq));
}

/* Return PST bonus for a piece on a given 0x88 square, from WHITE's perspective */
static int pst_score(uint8_t piece, uint8_t sq)
{
    if (!IS_PIECE(piece)) return 0;
    uint8_t idx = (PIECE_COLOR(piece) == WHITE) ? sq : mirror_sq(sq);
    int sign = (PIECE_COLOR(piece) == WHITE) ? 1 : -1;
    switch (PIECE_TYPE(piece)) {
        case 1: return sign * PST_PAWN  [idx];
        case 2: return sign * PST_KNIGHT[idx];
        case 3: return sign * PST_BISHOP[idx];
        case 4: return sign * PST_ROOK  [idx];
        case 5: return sign * PST_QUEEN [idx];
        case 6: return sign * PST_KING_MG[idx];
        default: return 0;
    }
}

/* ============================================================
 * ZOBRIST HASHING
 * ============================================================ */

static uint64_t s_zobrist_piece[128][16];
static uint64_t s_zobrist_ep[16];
static uint64_t s_zobrist_castling[16];
static uint64_t s_zobrist_black;

static void zobrist_init(void)
{
    /* Xorshift64 PRNG — deterministic seed for reproducibility */
    uint64_t state = 0xDEADBEEFCAFEBABEULL;
    #define RAND64() ({ state ^= state<<13; state ^= state>>7; \
                        state ^= state<<17; state; })

    for (int sq = 0; sq < 128; sq++)
        for (int p = 0; p < 16; p++)
            s_zobrist_piece[sq][p] = RAND64();
    for (int i = 0; i < 16; i++) {
        s_zobrist_ep[i]       = RAND64();
        s_zobrist_castling[i] = RAND64();
    }
    s_zobrist_black = RAND64();
    #undef RAND64
}

/* ============================================================
 * TRANSPOSITION TABLE
 * ============================================================ */

typedef struct {
    uint64_t key;
    int16_t  score;
    uint8_t  depth;
    uint8_t  type;   /* HT_EXACT / HT_LOWER / HT_UPPER */
    uint8_t  from;
    uint8_t  to;
    uint8_t  promo;
    uint8_t  _pad;
} ht_entry_t;        /* 16 bytes */

static ht_entry_t *s_ht      = NULL;
static uint32_t    s_ht_mask = 0;   /* power-of-2 size minus 1 */

static void ht_alloc(uint32_t kb)
{
    uint32_t entries = (kb * 1024U) / sizeof(ht_entry_t);
    /* Round down to power of 2 */
    uint32_t po2 = 1;
    while (po2 * 2 <= entries) po2 <<= 1;

    if (s_ht) heap_caps_free(s_ht);
    s_ht = heap_caps_calloc(po2, sizeof(ht_entry_t),
                             MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!s_ht) {
        /* Fallback: 64 entries in internal RAM */
        s_ht   = calloc(64, sizeof(ht_entry_t));
        po2    = 64;
    }
    s_ht_mask = po2 - 1;
}

static inline ht_entry_t *ht_probe(uint64_t key)
{
    return s_ht ? &s_ht[key & s_ht_mask] : NULL;
}

static inline void ht_store(uint64_t key, int score, int depth,
                             int type, uint8_t from, uint8_t to, uint8_t promo)
{
    if (!s_ht) return;
    ht_entry_t *e = &s_ht[key & s_ht_mask];
    /* Always-replace strategy */
    e->key   = key;
    e->score = (int16_t)score;
    e->depth = (uint8_t)depth;
    e->type  = (uint8_t)type;
    e->from  = from;
    e->to    = to;
    e->promo = promo;
}

/* ============================================================
 * BOARD STATE
 * ============================================================ */

typedef struct {
    uint8_t  board[128];
    uint8_t  side;       /* WHITE or BLACK */
    uint8_t  castling;   /* CWK|CWQ|CBK|CBQ */
    uint8_t  ep_sq;      /* En passant target, NO_SQ if none */
    uint64_t hash;
} pos_t;

static pos_t s_pos;      /* Current live position */

/* Incrementally update hash on piece set/clear */
static inline void hash_piece(pos_t *p, uint8_t sq, uint8_t piece)
{
    p->hash ^= s_zobrist_piece[sq][(uint8_t)piece & 15];
}

/* Full hash rebuild from scratch */
static void rebuild_hash(pos_t *p)
{
    p->hash = 0;
    for (int sq = 0; sq < 128; sq++) {
        if (ON_BOARD(sq) && p->board[sq] != EMPTY)
            p->hash ^= s_zobrist_piece[sq][p->board[sq] & 15];
    }
    p->hash ^= s_zobrist_castling[p->castling & 15];
    if (p->ep_sq != NO_SQ)
        p->hash ^= s_zobrist_ep[SQ_FILE(p->ep_sq) & 15];
    if (p->side == BLACK)
        p->hash ^= s_zobrist_black;
}

/* ============================================================
 * FEN PARSING
 * ============================================================ */

static const char *START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static uint8_t fen_piece_code(char c)
{
    switch (c) {
        case 'P': return WP; case 'N': return WN;
        case 'B': return WB; case 'R': return WR;
        case 'Q': return WQ; case 'K': return WK;
        case 'p': return BP; case 'n': return BN;
        case 'b': return BB; case 'r': return BRK;
        case 'q': return BQ; case 'k': return BK;
    }
    return EMPTY;
}

static void pos_set_fen(pos_t *p, const char *fen)
{
    memset(p->board, 0, sizeof(p->board));
    p->castling = 0;
    p->ep_sq    = NO_SQ;

    /* ── Piece placement: FEN rank 8 down to rank 1 ─────────── */
    int rank = 7, file = 0;
    const char *ptr = fen;
    while (*ptr && *ptr != ' ') {
        if (*ptr == '/') {
            rank--;
            file = 0;
        } else if (*ptr >= '1' && *ptr <= '8') {
            file += *ptr - '0';
        } else {
            uint8_t pc = fen_piece_code(*ptr);
            if (pc && file < 8 && rank >= 0) {
                p->board[SQ(file, rank)] = pc;
            }
            file++;
        }
        ptr++;
    }

    /* ── Active color ────────────────────────────────────────── */
    while (*ptr == ' ') ptr++;
    p->side = (*ptr == 'b') ? BLACK : WHITE;
    while (*ptr && *ptr != ' ') ptr++;
    while (*ptr == ' ') ptr++;

    /* ── Castling ────────────────────────────────────────────── */
    while (*ptr && *ptr != ' ') {
        if      (*ptr == 'K') p->castling |= CWK;
        else if (*ptr == 'Q') p->castling |= CWQ;
        else if (*ptr == 'k') p->castling |= CBK;
        else if (*ptr == 'q') p->castling |= CBQ;
        ptr++;
    }
    while (*ptr == ' ') ptr++;

    /* ── En passant ──────────────────────────────────────────── */
    if (*ptr != '-') {
        int ep_file = ptr[0] - 'a';
        int ep_rank = ptr[1] - '1';
        if (ep_file >= 0 && ep_file < 8 && ep_rank >= 0 && ep_rank < 8)
            p->ep_sq = SQ(ep_file, ep_rank);
    }

    rebuild_hash(p);
}

/* ============================================================
 * MAKE / UNMAKE  (copy-make approach)
 * ============================================================ */

/* Apply a UCI move string ("e2e4", "e7e8q", etc.) to position p.
 * Returns false if the move string is malformed. */
static bool pos_apply_uci(pos_t *p, const char *mv)
{
    if (!mv || strlen(mv) < 4) return false;

    int ff = mv[0] - 'a', rf = mv[1] - '1';
    int ft = mv[2] - 'a', rt = mv[3] - '1';
    if (ff < 0 || ff > 7 || rf < 0 || rf > 7 ||
        ft < 0 || ft > 7 || rt < 0 || rt > 7) return false;

    uint8_t from     = SQ(ff, rf);
    uint8_t to       = SQ(ft, rt);
    uint8_t mover    = p->board[from];
    uint8_t captured = p->board[to];
    uint8_t mtype    = PIECE_TYPE(mover);

    /* Remove old hash contributions */
    p->hash ^= s_zobrist_castling[p->castling & 15];
    if (p->ep_sq != NO_SQ)
        p->hash ^= s_zobrist_ep[SQ_FILE(p->ep_sq) & 15];

    /* Move piece */
    hash_piece(p, from, mover);
    if (captured) hash_piece(p, to, captured);
    p->board[to]   = mover;
    p->board[from] = EMPTY;
    hash_piece(p, to, mover);

    /* ── Castling: move rook ─────────────────────────────────── */
    if (mtype == 6 /* KING */) {
        int df = ft - ff;
        if (df == 2) {
            /* Kingside */
            uint8_t rf_sq = SQ(7, rf), rt_sq = SQ(5, rf);
            hash_piece(p, rf_sq, p->board[rf_sq]);
            p->board[rt_sq]  = p->board[rf_sq];
            p->board[rf_sq]  = EMPTY;
            hash_piece(p, rt_sq, p->board[rt_sq]);
        } else if (df == -2) {
            /* Queenside */
            uint8_t rf_sq = SQ(0, rf), rt_sq = SQ(3, rf);
            hash_piece(p, rf_sq, p->board[rf_sq]);
            p->board[rt_sq]  = p->board[rf_sq];
            p->board[rf_sq]  = EMPTY;
            hash_piece(p, rt_sq, p->board[rt_sq]);
        }
        /* Revoke all castling for this side */
        if (p->side == WHITE) p->castling &= ~(CWK | CWQ);
        else                  p->castling &= ~(CBK | CBQ);
    }

    /* ── En passant capture ──────────────────────────────────── */
    p->ep_sq = NO_SQ;
    if (mtype == 1 /* PAWN */) {
        /* En passant capture */
        if (ft != ff && !captured) {
            int cap_rank = rf;   /* Captured pawn is on same rank as FROM */
            uint8_t cap_sq = SQ(ft, cap_rank);
            hash_piece(p, cap_sq, p->board[cap_sq]);
            p->board[cap_sq] = EMPTY;
        }
        /* Set new EP target on double push */
        int dr = rt - rf;
        if (dr == 2 || dr == -2) {
            p->ep_sq = SQ(ff, rf + dr / 2);
        }
        /* Promotion */
        if (mv[4] != '\0' && mv[4] != ' ') {
            uint8_t promo_type;
            switch (tolower(mv[4])) {
                case 'q': promo_type = 5; break;
                case 'r': promo_type = 4; break;
                case 'b': promo_type = 3; break;
                case 'n': promo_type = 2; break;
                default:  promo_type = 5; break;
            }
            hash_piece(p, to, p->board[to]);
            p->board[to] = MAKE_PIECE(promo_type, p->side);
            hash_piece(p, to, p->board[to]);
        }
    }

    /* ── Revoke castling rights on rook moves / captures ──────── */
    if (from == SQ(0,0) || to == SQ(0,0)) p->castling &= ~CWQ;
    if (from == SQ(7,0) || to == SQ(7,0)) p->castling &= ~CWK;
    if (from == SQ(0,7) || to == SQ(0,7)) p->castling &= ~CBQ;
    if (from == SQ(7,7) || to == SQ(7,7)) p->castling &= ~CBK;

    /* ── Update hash with new castle/EP state ────────────────── */
    p->hash ^= s_zobrist_castling[p->castling & 15];
    if (p->ep_sq != NO_SQ)
        p->hash ^= s_zobrist_ep[SQ_FILE(p->ep_sq) & 15];

    /* ── Switch sides ───────────────────────────────────────── */
    p->side ^= 1;
    p->hash ^= s_zobrist_black;
    return true;
}

/* ============================================================
 * ATTACK DETECTION
 * ============================================================ */

static bool sq_attacked(const pos_t *p, uint8_t sq, uint8_t by_color)
{
    uint8_t ep = MAKE_PIECE(1, by_color);   /* enemy pawn   */
    uint8_t en = MAKE_PIECE(2, by_color);   /* enemy knight */
    uint8_t eb = MAKE_PIECE(3, by_color);   /* enemy bishop */
    uint8_t er = MAKE_PIECE(4, by_color);   /* enemy rook   */
    uint8_t eq = MAKE_PIECE(5, by_color);   /* enemy queen  */
    uint8_t ek = MAKE_PIECE(6, by_color);   /* enemy king   */

    /* ── Pawn attacks ───────────────────────────────────────── */
    if (by_color == WHITE) {
        /* White pawn at sq-15 or sq-17 attacks sq */
        uint8_t s1 = sq - 15, s2 = sq - 17;
        if (ON_BOARD(s1) && p->board[s1] == ep) return true;
        if (ON_BOARD(s2) && p->board[s2] == ep) return true;
    } else {
        /* Black pawn at sq+15 or sq+17 attacks sq */
        uint8_t s1 = sq + 15, s2 = sq + 17;
        if (ON_BOARD(s1) && p->board[s1] == ep) return true;
        if (ON_BOARD(s2) && p->board[s2] == ep) return true;
    }

    /* ── Knight attacks ─────────────────────────────────────── */
    static const int8_t N_DELTA[8] = {33, 31, 18, 14, -14, -18, -31, -33};
    for (int i = 0; i < 8; i++) {
        uint8_t ns = sq + (uint8_t)N_DELTA[i];
        if (ON_BOARD(ns) && p->board[ns] == en) return true;
    }

    /* ── Diagonal rays (bishop + queen) ─────────────────────── */
    static const int8_t D_DELTA[4] = {17, 15, -15, -17};
    for (int d = 0; d < 4; d++) {
        uint8_t s = sq;
        for (;;) {
            s += (uint8_t)D_DELTA[d];
            if (!ON_BOARD(s)) break;
            if (p->board[s] == eb || p->board[s] == eq) return true;
            if (p->board[s] != EMPTY) break;
        }
    }

    /* ── Orthogonal rays (rook + queen) ─────────────────────── */
    static const int8_t O_DELTA[4] = {16, -16, 1, -1};
    for (int d = 0; d < 4; d++) {
        uint8_t s = sq;
        for (;;) {
            s += (uint8_t)O_DELTA[d];
            if (!ON_BOARD(s)) break;
            if (p->board[s] == er || p->board[s] == eq) return true;
            if (p->board[s] != EMPTY) break;
        }
    }

    /* ── King adjacency ─────────────────────────────────────── */
    static const int8_t K_DELTA[8] = {16,-16,1,-1,17,15,-15,-17};
    for (int i = 0; i < 8; i++) {
        uint8_t ks = sq + (uint8_t)K_DELTA[i];
        if (ON_BOARD(ks) && p->board[ks] == ek) return true;
    }
    return false;
}

static inline bool in_check(const pos_t *p)
{
    /* Find king of side-to-move */
    uint8_t our_king = MAKE_PIECE(6, p->side);
    for (int sq = 0; sq < 128; sq++) {
        if (ON_BOARD(sq) && p->board[sq] == our_king) {
            return sq_attacked(p, (uint8_t)sq, p->side ^ 1);
        }
    }
    return false; /* No king found — shouldn't happen */
}

/* ============================================================
 * EVALUATION
 * Returns score from the current side's perspective.
 * ============================================================ */

static int evaluate(const pos_t *p)
{
    int score = 0;
    for (int sq = 0; sq < 128; sq++) {
        if (!ON_BOARD(sq)) continue;
        uint8_t pc = p->board[sq];
        if (pc == EMPTY) continue;
        int mat = PIECE_VAL[pc];
        int pst = pst_score(pc, (uint8_t)sq);
        if (PIECE_COLOR(pc) == WHITE) score += mat + pst;
        else                          score -= mat + pst;
    }
    /* Flip for current side */
    return (p->side == WHITE) ? score : -score;
}

/* ============================================================
 * MOVE GENERATION AND ORDERING
 * ============================================================ */

typedef struct {
    uint8_t from;
    uint8_t to;
    uint8_t promo;   /* Promotion piece type (1-6), 0 if none */
    int16_t score;   /* Move ordering score */
} smove_t;

/* Generate all pseudo-legal moves for position p.
 * Returns count; moves[] must hold at least 256 entries. */
static int gen_moves(const pos_t *p, smove_t *moves)
{
    int n = 0;
    uint8_t our_color = p->side;

    /* Direction tables */
    static const int8_t B_DIR[] = { 17, 15,-15,-17, 0 };
    static const int8_t R_DIR[] = { 16, 1, -1, -16, 0 };
    static const int8_t Q_DIR[] = { 17,16,15,1,-1,-15,-16,-17, 0 };
    static const int8_t K_DIR[] = { 17,16,15,1,-1,-15,-16,-17, 0 };
    static const int8_t N_DIR[] = { 33,31,18,14,-14,-18,-31,-33, 0 };

    #define ADD_MOVE(F,T,PR) do { \
        if (n < 255) { moves[n].from=(F); moves[n].to=(T); \
                       moves[n].promo=(PR); moves[n].score=0; n++; } \
    } while(0)

    for (int sq = 0; sq < 128; sq++) {
        if (!ON_BOARD(sq)) continue;
        uint8_t pc = p->board[sq];
        if (pc == EMPTY || PIECE_COLOR(pc) != our_color) continue;

        uint8_t ptype = PIECE_TYPE(pc);

        /* ── Pawns ────────────────────────────────────────────── */
        if (ptype == 1) {
            int dir  = (our_color == WHITE) ? 16 : -16;
            int start_rank = (our_color == WHITE) ? 1 : 6;
            int promo_rank = (our_color == WHITE) ? 7 : 0;

            uint8_t fwd = (uint8_t)((int)sq + dir);
            if (ON_BOARD(fwd) && p->board[fwd] == EMPTY) {
                if (SQ_RANK(fwd) == promo_rank) {
                    for (int pr = 5; pr >= 2; pr--) ADD_MOVE(sq, fwd, pr);
                } else {
                    ADD_MOVE(sq, fwd, 0);
                    /* Double push from start rank */
                    if (SQ_RANK(sq) == start_rank) {
                        uint8_t fwd2 = (uint8_t)((int)fwd + dir);
                        if (ON_BOARD(fwd2) && p->board[fwd2] == EMPTY)
                            ADD_MOVE(sq, fwd2, 0);
                    }
                }
            }
            /* Captures (including en passant) */
            int8_t cap_dir[2] = { (int8_t)(dir + 1), (int8_t)(dir - 1) };
            for (int cd = 0; cd < 2; cd++) {
                uint8_t cs = (uint8_t)((int)sq + cap_dir[cd]);
                if (!ON_BOARD(cs)) continue;
                bool is_enemy = (p->board[cs] != EMPTY &&
                                 PIECE_COLOR(p->board[cs]) != our_color);
                bool is_ep    = (cs == p->ep_sq);
                if (is_enemy || is_ep) {
                    if (SQ_RANK(cs) == promo_rank) {
                        for (int pr = 5; pr >= 2; pr--) ADD_MOVE(sq, cs, pr);
                    } else {
                        ADD_MOVE(sq, cs, 0);
                    }
                }
            }
            continue;
        }

        /* ── Knights ──────────────────────────────────────────── */
        if (ptype == 2) {
            for (int i = 0; N_DIR[i]; i++) {
                uint8_t to = (uint8_t)((int)sq + N_DIR[i]);
                if (ON_BOARD(to) && !SAME_COLOR(pc, p->board[to]))
                    ADD_MOVE(sq, to, 0);
            }
            continue;
        }

        /* ── Sliding pieces ───────────────────────────────────── */
        const int8_t *dirs = NULL;
        bool sliding = true;
        if (ptype == 3) dirs = B_DIR;
        else if (ptype == 4) dirs = R_DIR;
        else if (ptype == 5) dirs = Q_DIR;
        else if (ptype == 6) { dirs = K_DIR; sliding = false; }

        if (!dirs) continue;
        for (int i = 0; dirs[i]; i++) {
            uint8_t to = sq;
            do {
                to = (uint8_t)((int)to + dirs[i]);
                if (!ON_BOARD(to)) break;
                if (SAME_COLOR(pc, p->board[to])) break;
                ADD_MOVE(sq, to, 0);
                if (p->board[to] != EMPTY) break; /* Blocked after capture */
            } while (sliding);
        }
    }

    /* ── Castling ─────────────────────────────────────────────── */
    if (our_color == WHITE) {
        uint8_t enemy = BLACK;
        if ((p->castling & CWK) &&
            p->board[SQ(5,0)] == EMPTY && p->board[SQ(6,0)] == EMPTY &&
            !sq_attacked(p, SQ(4,0), enemy) &&
            !sq_attacked(p, SQ(5,0), enemy) &&
            !sq_attacked(p, SQ(6,0), enemy))
            ADD_MOVE(SQ(4,0), SQ(6,0), 0);

        if ((p->castling & CWQ) &&
            p->board[SQ(3,0)] == EMPTY && p->board[SQ(2,0)] == EMPTY &&
            p->board[SQ(1,0)] == EMPTY &&
            !sq_attacked(p, SQ(4,0), enemy) &&
            !sq_attacked(p, SQ(3,0), enemy) &&
            !sq_attacked(p, SQ(2,0), enemy))
            ADD_MOVE(SQ(4,0), SQ(2,0), 0);
    } else {
        uint8_t enemy = WHITE;
        if ((p->castling & CBK) &&
            p->board[SQ(5,7)] == EMPTY && p->board[SQ(6,7)] == EMPTY &&
            !sq_attacked(p, SQ(4,7), enemy) &&
            !sq_attacked(p, SQ(5,7), enemy) &&
            !sq_attacked(p, SQ(6,7), enemy))
            ADD_MOVE(SQ(4,7), SQ(6,7), 0);

        if ((p->castling & CBQ) &&
            p->board[SQ(3,7)] == EMPTY && p->board[SQ(2,7)] == EMPTY &&
            p->board[SQ(1,7)] == EMPTY &&
            !sq_attacked(p, SQ(4,7), enemy) &&
            !sq_attacked(p, SQ(3,7), enemy) &&
            !sq_attacked(p, SQ(2,7), enemy))
            ADD_MOVE(SQ(4,7), SQ(2,7), 0);
    }

    #undef ADD_MOVE
    return n;
}

/* Score moves for ordering: hash move > captures (MVV-LVA) > quiet */
static void score_moves(smove_t *moves, int n, uint8_t ht_from, uint8_t ht_to,
                         const pos_t *p)
{
    for (int i = 0; i < n; i++) {
        smove_t *m = &moves[i];
        if (m->from == ht_from && m->to == ht_to) {
            m->score = 30000; /* Hash move first */
        } else {
            uint8_t cap = p->board[m->to];
            if (cap != EMPTY) {
                /* MVV-LVA: victim value - attacker value/10 */
                int16_t v = PIECE_VAL[cap];
                int16_t a = PIECE_VAL[p->board[m->from]] / 10;
                m->score = (int16_t)(10000 + v - a);
            } else {
                m->score = 0;
            }
        }
    }
}

/* Simple insertion sort (typically ≤ 40 moves, negligible overhead) */
static void sort_moves(smove_t *moves, int n)
{
    for (int i = 1; i < n; i++) {
        smove_t tmp = moves[i];
        int j = i - 1;
        while (j >= 0 && moves[j].score < tmp.score) {
            moves[j+1] = moves[j];
            j--;
        }
        moves[j+1] = tmp;
    }
}

/* Build UCI string for a move */
static void move_to_uci(smove_t m, char *out)
{
    out[0] = (char)('a' + SQ_FILE(m.from));
    out[1] = (char)('1' + SQ_RANK(m.from));
    out[2] = (char)('a' + SQ_FILE(m.to));
    out[3] = (char)('1' + SQ_RANK(m.to));
    if (m.promo) {
        static const char pc[] = ".pnbrqk";
        out[4] = pc[m.promo];
        out[5] = '\0';
    } else {
        out[4] = '\0';
    }
}

/* ============================================================
 * SEARCH
 * ============================================================ */

static volatile bool  s_stop;
static smove_t        s_root_best;   /* Best move from completed iteration */
static long           s_nodes;

/* Quiescence search — only captures */
static int quiesce(pos_t pos, int alpha, int beta)
{
    s_nodes++;
    if (s_stop) return alpha;

    int stand_pat = evaluate(&pos);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    smove_t moves[64];
    int n = gen_moves(&pos, moves);
    score_moves(moves, n, NO_SQ, NO_SQ, &pos);
    sort_moves(moves, n);

    for (int i = 0; i < n; i++) {
        if (pos.board[moves[i].to] == EMPTY) continue; /* Quiet move */
        pos_t child = pos;
        char mv_str[6];
        move_to_uci(moves[i], mv_str);
        pos_apply_uci(&child, mv_str);
        if (in_check(&child) && child.side != pos.side) {
            /* Illegal move (left own king in check) — skip */
            /* Actually after apply, child.side is flipped.
             * We need to check if the side that JUST MOVED is in check. */
            pos_t check_p = child;
            check_p.side ^= 1;
            if (in_check(&check_p)) continue;
        }
        int score = -quiesce(child, -beta, -alpha);
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

/* Negamax with alpha-beta, returns score from current side's perspective */
static int negamax(pos_t pos, int alpha, int beta, int depth, int ply)
{
    s_nodes++;
    if (s_stop) return 0;

    /* TT probe */
    ht_entry_t *tte = ht_probe(pos.hash);
    uint8_t ht_from = NO_SQ, ht_to = NO_SQ;
    if (tte && tte->key == pos.hash) {
        ht_from = tte->from;
        ht_to   = tte->to;
        if (tte->depth >= depth) {
            int s = tte->score;
            if (tte->type == HT_EXACT) return s;
            if (tte->type == HT_LOWER && s >= beta)  return s;
            if (tte->type == HT_UPPER && s <= alpha) return s;
        }
    }

    if (depth <= 0) return quiesce(pos, alpha, beta);

    smove_t moves[256];
    int n = gen_moves(&pos, moves);
    score_moves(moves, n, ht_from, ht_to, &pos);
    sort_moves(moves, n);

    int legal_moves = 0;
    int best_score  = -INF;
    smove_t best_mv = {0, 0, 0, 0};
    int orig_alpha  = alpha;

    for (int i = 0; i < n; i++) {
        pos_t child = pos;
        char mv_str[6];
        move_to_uci(moves[i], mv_str);
        pos_apply_uci(&child, mv_str);

        /* Legality check: did we leave own king in check? */
        {
            pos_t check_pos = child;
            check_pos.side ^= 1;   /* Switch back to the side that just moved */
            if (in_check(&check_pos)) continue;
        }
        legal_moves++;

        int score = -negamax(child, -beta, -alpha, depth - 1, ply + 1);
        if (s_stop) break;

        if (score > best_score) {
            best_score = score;
            best_mv    = moves[i];
            if (ply == 0) s_root_best = moves[i];
        }
        if (score > alpha) alpha = score;
        if (alpha >= beta) break;  /* Beta cutoff */
    }

    if (legal_moves == 0) {
        /* No legal moves: checkmate or stalemate */
        best_score = in_check(&pos) ? -(MATE_BASE - ply) : 0;
    }


    int tt_type = (best_score <= orig_alpha) ? HT_UPPER
                : (best_score >= beta)       ? HT_LOWER
                :                              HT_EXACT;
    ht_store(pos.hash, best_score, depth, tt_type,
             best_mv.from, best_mv.to, best_mv.promo);

    return best_score;
}

/* Iterative deepening search.  Fills s_root_best with best move. */
static int search(int max_depth, long max_nodes)
{
    s_stop  = false;
    s_nodes = 0;
    int final_score = 0;
    memset(&s_root_best, 0, sizeof(s_root_best));

    for (int depth = 1; depth <= max_depth && !s_stop; depth++) {
        smove_t prev_best = s_root_best;
        int score = negamax(s_pos, -INF, INF, depth, 0);
        if (s_stop) {
            /* Restore best move from completed shallower search */
            s_root_best = prev_best;
            break;
        }
        final_score = score;

        /* Emit info line */
        char info[128];
        char mv[6];
        move_to_uci(s_root_best, mv);
        snprintf(info, sizeof(info),
                 "info depth %d score cp %d nodes %ld pv %s",
                 depth, final_score, s_nodes, mv);
        uci_engine_puts(info);

        if (IS_MATE(score)) break;            /* Forced mate found */
        if (max_nodes > 0 && s_nodes >= max_nodes) break;
    }
    return final_score;
}

/* ============================================================
 * UCI LOOP
 * ============================================================ */

void mcumax_uci_loop(void)
{
    /* Initialise Zobrist tables and allocate default hash */
    zobrist_init();
    ht_alloc(MCU_MAX_HASH_KB);

    /* Set up starting position */
    pos_set_fen(&s_pos, START_FEN);

    char line[512];
    char reply[256];

    for (;;) {
        uci_engine_getline(line, sizeof(line));

        /* ── uci ───────────────────────────────────────────── */
        if (strncmp(line, "uci", 3) == 0) {
            uci_engine_puts("id name mcu-max");
            uci_engine_puts("id author H.G.Muller / Gissio / chess-board-fw");
            snprintf(reply, sizeof(reply),
                     "option name Hash type spin default %d min 1 max 256",
                     MCU_MAX_HASH_KB / 1024);
            uci_engine_puts(reply);
            snprintf(reply, sizeof(reply),
                     "option name Depth type spin default %d min 1 max 15",
                     SF_SEARCH_DEPTH);
            uci_engine_puts(reply);
            uci_engine_puts("uciok");

        /* ── setoption ─────────────────────────────────────── */
        } else if (strncmp(line, "setoption name Hash value ", 26) == 0) {
            int mb = atoi(line + 26);
            if (mb >= 1 && mb <= 256) ht_alloc((uint32_t)mb * 1024);

        /* ── isready ───────────────────────────────────────── */
        } else if (strncmp(line, "isready", 7) == 0) {
            uci_engine_puts("readyok");

        /* ── ucinewgame ────────────────────────────────────── */
        } else if (strncmp(line, "ucinewgame", 10) == 0) {
            if (s_ht) memset(s_ht, 0, (s_ht_mask + 1) * sizeof(ht_entry_t));
            pos_set_fen(&s_pos, START_FEN);

        /* ── position ──────────────────────────────────────── */
        } else if (strncmp(line, "position ", 9) == 0) {
            const char *p = line + 9;

            if (strncmp(p, "startpos", 8) == 0) {
                pos_set_fen(&s_pos, START_FEN);
                p += 8;
            } else if (strncmp(p, "fen ", 4) == 0) {
                p += 4;
                pos_set_fen(&s_pos, p);
                /* Advance past the FEN fields (6 space-separated tokens) */
                int spaces = 0;
                while (*p && spaces < 6) {
                    if (*p == ' ') spaces++;
                    p++;
                }
            }

            /* Apply move list */
            if (*p && strncmp(p, " moves ", 7) == 0) p += 7;
            else if (*p && strncmp(p, "moves ", 6) == 0) p += 6;
            while (*p) {
                while (*p == ' ') p++;
                if (!*p) break;
                char mv[6] = {0};
                int mi = 0;
                while (*p && *p != ' ' && mi < 5) mv[mi++] = *p++;
                mv[mi] = '\0';
                pos_apply_uci(&s_pos, mv);
            }

        /* ── go ────────────────────────────────────────────── */
        } else if (strncmp(line, "go", 2) == 0) {
            int depth     = SF_SEARCH_DEPTH;
            long max_nodes = 0;

            /* Parse "go depth N" or "go nodes N" */
            const char *tok = strstr(line, "depth ");
            if (tok) depth = atoi(tok + 6);
            tok = strstr(line, "nodes ");
            if (tok) max_nodes = atol(tok + 6);

            search(depth, max_nodes);

            char mv_str[6] = "0000";
            if (s_root_best.from != 0 || s_root_best.to != 0) {
                move_to_uci(s_root_best, mv_str);
            }
            snprintf(reply, sizeof(reply), "bestmove %s", mv_str);
            uci_engine_puts(reply);

        /* ── stop ──────────────────────────────────────────── */
        } else if (strncmp(line, "stop", 4) == 0) {
            s_stop = true;

        /* ── quit ──────────────────────────────────────────── */
        } else if (strncmp(line, "quit", 4) == 0) {
            return;
        }

        taskYIELD();  /* Be polite to other FreeRTOS tasks */
    }
}
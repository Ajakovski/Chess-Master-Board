#include "board.h"
#include "config.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>



void board_init_start(board_t *b)
{
    memset(b, 0, sizeof(*b));

    
    static const piece_type_t back_row[8] = {
        PT_ROOK, PT_KNIGHT, PT_BISHOP, PT_QUEEN,
        PT_KING, PT_BISHOP, PT_KNIGHT, PT_ROOK
    };
    for (int f = 0; f < 8; f++) {
        b->pieces[SQ(f, 0)] = (piece_t){ back_row[f], PC_WHITE };
        b->pieces[SQ(f, 1)] = (piece_t){ PT_PAWN,     PC_WHITE };
        b->pieces[SQ(f, 6)] = (piece_t){ PT_PAWN,     PC_BLACK };
        b->pieces[SQ(f, 7)] = (piece_t){ back_row[f], PC_BLACK };
    }


    b->castling    = CASTLE_WK | CASTLE_WQ | CASTLE_BK | CASTLE_BQ;
    b->en_passant  = SQ_NONE;
    b->active      = PC_WHITE;
    b->halfmove    = 0;
    b->fullmove    = 1;
}

uint64_t board_occupied(const board_t *b)
{
    uint64_t mask = 0;
    for (int sq = 0; sq < 64; sq++) {
        if (b->pieces[sq].type != PT_NONE) {
            mask |= (1ULL << sq);
        }
    }
    return mask;
}

int board_king_sq(const board_t *b, piece_color_t color)
{
    for (int sq = 0; sq < 64; sq++) {
        if (b->pieces[sq].type  == PT_KING &&
            b->pieces[sq].color == color) {
            return sq;
        }
    }
    return SQ_NONE;
}


bool board_sq_attacked(const board_t *b, int sq, piece_color_t by)
{
    
    int pawn_dir = (by == PC_WHITE) ? 1 : -1;
    int pf = SQ_FILE(sq);
    int pr = SQ_RANK(sq) - pawn_dir; 
    if (pr >= 0 && pr < 8) {
        if (pf > 0) {
            int ps = SQ(pf-1, pr);
            if (b->pieces[ps].type  == PT_PAWN &&
                b->pieces[ps].color == by) return true;
        }
        if (pf < 7) {
            int ps = SQ(pf+1, pr);
            if (b->pieces[ps].type  == PT_PAWN &&
                b->pieces[ps].color == by) return true;
        }
    }


    static const int kdx[8] = {1,2, 2, 1,-1,-2,-2,-1};
    static const int kdy[8] = {2,1,-1,-2,-2,-1, 1, 2};
    for (int i = 0; i < 8; i++) {
        int nf = pf + kdx[i];
        int nr = SQ_RANK(sq) + kdy[i];
        if (nf >= 0 && nf < 8 && nr >= 0 && nr < 8) {
            int ns = SQ(nf, nr);
            if (b->pieces[ns].type  == PT_KNIGHT &&
                b->pieces[ns].color == by) return true;
        }
    }


    static const int diag_df[4] = { 1,-1, 1,-1};
    static const int diag_dr[4] = { 1, 1,-1,-1};
    for (int d = 0; d < 4; d++) {
        int f2 = pf + diag_df[d];
        int r2 = SQ_RANK(sq) + diag_dr[d];
        while (f2 >= 0 && f2 < 8 && r2 >= 0 && r2 < 8) {
            int s2 = SQ(f2, r2);
            if (b->pieces[s2].type != PT_NONE) {
                if (b->pieces[s2].color == by &&
                    (b->pieces[s2].type == PT_BISHOP ||
                     b->pieces[s2].type == PT_QUEEN)) return true;
                break;
            }
            f2 += diag_df[d]; r2 += diag_dr[d];
        }
    }

    static const int orth_df[4] = { 1,-1, 0, 0};
    static const int orth_dr[4] = { 0, 0, 1,-1};
    for (int d = 0; d < 4; d++) {
        int f2 = pf + orth_df[d];
        int r2 = SQ_RANK(sq) + orth_dr[d];
        while (f2 >= 0 && f2 < 8 && r2 >= 0 && r2 < 8) {
            int s2 = SQ(f2, r2);
            if (b->pieces[s2].type != PT_NONE) {
                if (b->pieces[s2].color == by &&
                    (b->pieces[s2].type == PT_ROOK ||
                     b->pieces[s2].type == PT_QUEEN)) return true;
                break;
            }
            f2 += orth_df[d]; r2 += orth_dr[d];
        }
    }

   
    {
        int rf = pf, rr = SQ_RANK(sq);
        for (int df = -1; df <= 1; df++) {
            for (int dr = -1; dr <= 1; dr++) {
                if (df == 0 && dr == 0) continue;
                int kf = rf + df, kr = rr + dr;
                if (kf >= 0 && kf < 8 && kr >= 0 && kr < 8) {
                    int ks = SQ(kf, kr);
                    if (b->pieces[ks].type  == PT_KING &&
                        b->pieces[ks].color == by) return true;
                }
            }
        }
    }
    return false;
}

bool board_in_check(const board_t *b, piece_color_t color)
{
    int ksq = board_king_sq(b, color);
    if (ksq == SQ_NONE) return false;
    piece_color_t enemy = (color == PC_WHITE) ? PC_BLACK : PC_WHITE;
    return board_sq_attacked(b, ksq, enemy);
}

//Apply move

void board_apply_move(board_t *b, move_t m)
{
    int      from  = m.from;
    int      to    = m.to;
    piece_t  mover = b->pieces[from];
    piece_t  cap   = b->pieces[to];

    if (mover.type == PT_PAWN || cap.type != PT_NONE) {
        b->halfmove = 0;
    } else {
        b->halfmove++;
    }

//Default mover
    b->pieces[to]   = mover;
    b->pieces[from] = (piece_t){ PT_NONE, PC_WHITE };
    if (mover.type == PT_KING) {
        int df = SQ_FILE(to) - SQ_FILE(from);
        if (df == 2) {
            // Kingside
            int rook_from = SQ(7, SQ_RANK(from));
            int rook_to   = SQ(5, SQ_RANK(from));
            b->pieces[rook_to]   = b->pieces[rook_from];
            b->pieces[rook_from] = (piece_t){ PT_NONE, PC_WHITE };
        } else if (df == -2) {
            // Queenside
            int rook_from = SQ(0, SQ_RANK(from));
            int rook_to   = SQ(3, SQ_RANK(from));
            b->pieces[rook_to]   = b->pieces[rook_from];
            b->pieces[rook_from] = (piece_t){ PT_NONE, PC_WHITE };
        }
        if (mover.color == PC_WHITE) {
            b->castling &= ~(CASTLE_WK | CASTLE_WQ);
        } else {
            b->castling &= ~(CASTLE_BK | CASTLE_BQ);
        }
    }

//En passant
    if (mover.type == PT_PAWN && to == b->en_passant) {
        int cap_rank = SQ_RANK(to) + ((mover.color == PC_WHITE) ? -1 : 1);
        int cap_sq   = SQ(SQ_FILE(to), cap_rank);
        b->pieces[cap_sq] = (piece_t){ PT_NONE, PC_WHITE };
    }


    b->en_passant = SQ_NONE;
    if (mover.type == PT_PAWN) {
        int dr = SQ_RANK(to) - SQ_RANK(from);
        if (dr == 2 || dr == -2) {
            b->en_passant = (int8_t)SQ(SQ_FILE(from),
                             (SQ_RANK(from) + SQ_RANK(to)) / 2);
        }
    }

    if (m.promo != PT_NONE) {
        b->pieces[to].type = m.promo;
    }

//Castling right revoke
    if (from == SQ(0,0) || to == SQ(0,0)) b->castling &= ~CASTLE_WQ;
    if (from == SQ(7,0) || to == SQ(7,0)) b->castling &= ~CASTLE_WK;
    if (from == SQ(0,7) || to == SQ(0,7)) b->castling &= ~CASTLE_BQ;
    if (from == SQ(7,7) || to == SQ(7,7)) b->castling &= ~CASTLE_BK;

    if (b->active == PC_BLACK) {
        b->fullmove++;
    }
    b->active = (b->active == PC_WHITE) ? PC_BLACK : PC_WHITE;
}

//FEN
char piece_to_fen_char(piece_t p)
{
    if (p.type == PT_NONE) return '.';
    static const char types[] = ".pnbrqk";
    char c = types[p.type];
    return (p.color == PC_WHITE) ? (char)toupper(c) : c;
}

void board_to_fen(const board_t *b, char *buf, size_t len)
{
    char *p = buf;
    char *end = buf + len - 1;
    for (int r = 7; r >= 0 && p < end; r--) {
        int empty = 0;
        for (int f = 0; f < 8; f++) {
            piece_t piece = b->pieces[SQ(f, r)];
            if (piece.type == PT_NONE) {
                empty++;
            } else {
                if (empty > 0) { *p++ = (char)('0' + empty); empty = 0; }
                *p++ = piece_to_fen_char(piece);
            }
        }
        if (empty > 0) *p++ = (char)('0' + empty);
        if (r > 0)     *p++ = '/';
    }
    p += snprintf(p, (size_t)(end - p), " %c ", (b->active == PC_WHITE) ? 'w' : 'b');

    if (!(b->castling)) {
        if (p < end) *p++ = '-';
    } else {
        if ((b->castling & CASTLE_WK) && p < end) *p++ = 'K';
        if ((b->castling & CASTLE_WQ) && p < end) *p++ = 'Q';
        if ((b->castling & CASTLE_BK) && p < end) *p++ = 'k';
        if ((b->castling & CASTLE_BQ) && p < end) *p++ = 'q';
    }

    //en passant assign target
    if (b->en_passant == SQ_NONE) {
        p += snprintf(p, (size_t)(end - p), " -");
    } else {
        char ep[3] = {
            (char)('a' + SQ_FILE(b->en_passant)),
            (char)('1' + SQ_RANK(b->en_passant)),
            '\0'
        };
        p += snprintf(p, (size_t)(end - p), " %s", ep);
    }

    snprintf(p, (size_t)(end - p), " %d %d", b->halfmove, b->fullmove);
    buf[len - 1] = '\0';
}



void move_to_uci(move_t m, char *out)
{
    if (MOVE_IS_NULL(m)) { strcpy(out, "0000"); return; }
    out[0] = (char)('a' + SQ_FILE(m.from));
    out[1] = (char)('1' + SQ_RANK(m.from));
    out[2] = (char)('a' + SQ_FILE(m.to));
    out[3] = (char)('1' + SQ_RANK(m.to));
    if (m.promo != PT_NONE) {
        static const char promo_chars[] = ".pnbrqk";
        out[4] = promo_chars[m.promo];
        out[5] = '\0';
    } else {
        out[4] = '\0';
    }
}

bool move_from_uci(const char *uci, move_t *out)
{
    if (!uci || strlen(uci) < 4) return false;
    int ff = uci[0] - 'a';
    int rf = uci[1] - '1';
    int ft = uci[2] - 'a';
    int rt = uci[3] - '1';
    if (ff < 0 || ff > 7 || rf < 0 || rf > 7 ||
        ft < 0 || ft > 7 || rt < 0 || rt > 7) return false;
    out->from  = (int8_t)SQ(ff, rf);
    out->to    = (int8_t)SQ(ft, rt);
    out->promo = PT_NONE;
    if (uci[4] != '\0') {
        switch (tolower(uci[4])) {
            case 'q': out->promo = PT_QUEEN;  break;
            case 'r': out->promo = PT_ROOK;   break;
            case 'b': out->promo = PT_BISHOP; break;
            case 'n': out->promo = PT_KNIGHT; break;
            default:  return false;
        }
    }
    return true;
}
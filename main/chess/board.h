#pragma once

#include "types.h"
#include <stdbool.h>
#include <stddef.h>

void board_init_start(board_t *b);

uint64_t board_occupied(const board_t *b);

void board_to_fen(const board_t *b, char *buf, size_t len);

void move_to_uci(move_t m, char *out);

bool move_from_uci(const char *uci, move_t *out);

char piece_to_fen_char(piece_t p);

bool board_sq_attacked(const board_t *b, int sq, piece_color_t by);

bool board_in_check(const board_t *b, piece_color_t color);

void board_apply_move(board_t *b, move_t m);

int board_king_sq(const board_t *b, piece_color_t color);
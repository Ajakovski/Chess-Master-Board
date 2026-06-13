#pragma once

#include "types.h"
#include "board.h"
#include <stdbool.h>

#define MAX_MOVES_PER_POS 256
void rules_init(void);

bool rules_is_legal(const board_t *b, int from, int to, piece_type_t promo);

int rules_gen_moves_from(const board_t *b, int from, move_t *moves);

int rules_gen_all_moves(const board_t *b, move_t *moves);

uint64_t rules_dest_mask(const board_t *b, int from);

bool rules_is_checkmate(const board_t *b);

bool rules_is_stalemate(const board_t *b);

bool rules_is_insufficient_material(const board_t *b);

static inline bool rules_is_50move(const board_t *b) { return b->halfmove>=100;}

uint64_t rules_capture_candidates(const board_t *b, int from, uint64_t sensor_occupied);
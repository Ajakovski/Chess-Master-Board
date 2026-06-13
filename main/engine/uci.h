#pragma once

#include "types.h"
#include "esp_err.h"
#include <stdbool.h>

esp_err_t uci_init(void);

void uci_send_position(const move_history_t *history);

void uci_go_depth(int depth);

bool uci_wait_bestmove(move_t *best_move, int *depth, int *score_cp, uint32_t timeout_ms);

void uci_stop(void);

bool uci_ready(void);

void stockfish_task(void *arg);
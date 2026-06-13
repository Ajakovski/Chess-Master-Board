#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

esp_err_t display_init(void);

void display_clear(void);

void display_update(int32_t clock_ms_w,
                    int32_t clocl_ms_b,
                    int active,
                    int move_number,
                    int battery_pct,
                    bool sf_thinking,
                    int sf_depth,
                    int sf_score_cp);

void display_message(const char *line1, const char *line2);
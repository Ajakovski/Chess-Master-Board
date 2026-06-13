#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "types.h"

esp_err_t leds_init(void);

void leds_write(const rgb_t leds[64]);

void leds_clear(void);

static inline void leds_set_sq(rgb_t buf[64], int sq, rgb_t color){
    if(sq >= 0 && sq<64){
        buf[SQ_LED(sq)] = color;
    }
}

void leds_fill(rgb_t buf[64], rgb_t color);
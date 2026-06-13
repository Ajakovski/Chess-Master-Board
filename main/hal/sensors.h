#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

esp_err_t sensors_init(void);

void sensors_read(uint64_t *occupied);

static inline uint64_t sensors_start_mask(void){

    return UINT64_C(0xFFFF0000000FFFF);
}
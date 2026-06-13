#pragma once

#include <stdint.h>
#include "esp_err.h"

esp_err_t battery_init(void);

uint8_t battery_percent(void);

void battery_task(void *arg);
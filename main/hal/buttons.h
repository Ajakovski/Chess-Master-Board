#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "types.h"

esp_err_t buttons_init(void);

void buttons_task(void *arg);

bool buttons_p1_raw(void);

bool buttons_p2_raw(void);
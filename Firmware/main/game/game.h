#pragma once 

#include "types.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

esp_err_t game_init(void);

void game_sensor_task (void *arg);
void game_logic_task (void *arg);
void game_led_task (void *arg);
void game_display_task (void *arg);

extern QueueHandle_t g_button_queue;
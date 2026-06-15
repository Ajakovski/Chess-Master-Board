
#include "config.h"
#include "sensors.h"
#include "leds.h"
#include "display.h"
#include "buttons.h"
#include "battery.h"
#include "uci.h"
#include "game.h"

#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

void app_main(void)
{
    ESP_LOGI(TAG, "Chess Board Firmware — ESP32-S3-WROOM-2");
    ESP_LOGI(TAG, "Flash: 32 MB  PSRAM: 16 MB  CPU: 240 MHz");

    /* NVS*/
    esp_err_t nvs_err = nvs_flash_init();
    if (nvs_err == ESP_ERR_NVS_NO_FREE_PAGES ||
        nvs_err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    // Hal
    ESP_ERROR_CHECK(sensors_init());
    ESP_ERROR_CHECK(leds_init());
    ESP_ERROR_CHECK(display_init());
    ESP_ERROR_CHECK(buttons_init());
    ESP_ERROR_CHECK(battery_init());

    display_message("CHESS BOARD", "Loading engine...");

    // UCI engine
    esp_err_t sf_err = uci_init();
    if (sf_err != ESP_OK) {
        ESP_LOGE(TAG, "Stockfish init failed — running without engine");
        display_message("ENGINE ERROR", "Continuing sans SF");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    // Moment na igra
    ESP_ERROR_CHECK(game_init());

    
    ESP_LOGI(TAG, "Scheduler running — %d tasks",
             (int)uxTaskGetNumberOfTasks());
}
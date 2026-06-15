#include "buttons.h"
#include "config.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <stdbool.h>

static const char *TAG = "buttons";

/// nternal debounce state
typedef struct {
    gpio_num_t pin;
    bool       pressed;          
    bool       last_raw;         
    uint32_t   last_change_ms;  
    uint32_t   press_time_ms;    
    bool       event_pending;    /* Button is pressed    */
} btn_state_t;

static btn_state_t s_p1;
static btn_state_t s_p2;

// Helper

static inline uint32_t now_ms(void)
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

static void btn_update(btn_state_t *b)
{
    bool raw = (gpio_get_level(b->pin) == 0);  /* active-LOW */
    uint32_t t = now_ms();

    if (raw != b->last_raw) {
        
        b->last_raw        = raw;
        b->last_change_ms  = t;
    }

    /* Confirm debounced state after BTN_DEBOUNCE_MS */
    if ((t - b->last_change_ms) >= BTN_DEBOUNCE_MS) {
        if (raw && !b->pressed) {
            /* Confirmed press */
            b->pressed        = true;
            b->press_time_ms  = t;
            b->event_pending  = true;
        } else if (!raw && b->pressed) {
            /* Confirmed release */
            b->pressed       = false;
        }
    }
}


esp_err_t buttons_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << PIN_BTN_P1) | (1ULL << PIN_BTN_P2),
        .mode        = GPIO_MODE_INPUT,
        .pull_up_en   =  GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));

    s_p1 = (btn_state_t){ .pin = PIN_BTN_P1 };
    s_p2 = (btn_state_t){ .pin = PIN_BTN_P2 };

    ESP_LOGI(TAG, "Buttons ready (GPIO%d, GPIO%d)", PIN_BTN_P1, PIN_BTN_P2);
    return ESP_OK;
}



void buttons_task(void *arg)
{
    QueueHandle_t q = *(QueueHandle_t *)arg;

    bool both_hold_reported = false;
    uint32_t both_start_ms  = 0;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(10));
        uint32_t t = now_ms();

        btn_update(&s_p1);
        btn_update(&s_p2);

        // kratko zadrzano*/
    
        if (!s_p1.pressed && s_p1.event_pending) {
        uint32_t dur = t - s_p1.press_time_ms;
            if (dur >= BTN_DEBOUNCE_MS && dur < BTN_SHORT_MAX_MS) {
                button_event_t ev = BTN_P1_SHORT;
                xQueueSend(q, &ev, 0);
            }
            s_p1.event_pending = false;
        }
        if (!s_p2.pressed && s_p2.event_pending) {
            uint32_t dur = t - s_p2.press_time_ms;
            if (dur >= BTN_DEBOUNCE_MS && dur < BTN_SHORT_MAX_MS) {
                button_event_t ev = BTN_P2_SHORT;
                xQueueSend(q, &ev, 0);
            }
            s_p2.event_pending = false;
        }

        // Dvete zadrzani
        if (s_p1.pressed && s_p2.pressed) {
            if (!both_hold_reported) {
                
                uint32_t later = (s_p1.press_time_ms > s_p2.press_time_ms)
                        ? s_p1.press_time_ms : s_p2.press_time_ms;
                if (both_start_ms == 0) {
                    both_start_ms = later;
                }
                if ((t - both_start_ms) >= SLEEP_HOLD_BOTH_MS) {
                    button_event_t ev = BTN_BOTH_HOLD;
                    xQueueSend(q, &ev, 0);
                    both_hold_reported = true;
   
                    s_p1.event_pending = false;
                     s_p2.event_pending = false;
                }
            }
        } else {
            //reset hold tracking 
            both_hold_reported = false;
            both_start_ms      = 0;
        }
    }
}



bool buttons_p1_raw(void) { return s_p1.pressed; }
bool buttons_p2_raw(void) { return s_p2.pressed; }
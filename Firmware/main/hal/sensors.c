#include "sensors.h"
#include "config.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "soc/gpio_reg.h"
#include "rom/ets_sys.h"

static const char *TAG = "sensors";


#define _SET(pin)  REG_WRITE(GPIO_OUT_W1TS_REG, 1UL << (pin))
#define _CLR(pin)  REG_WRITE(GPIO_OUT_W1TC_REG, 1UL << (pin))
#define _RD(pin) ((REG_READ(GPIO_IN_REG) >> (pin)) & 1U)

// sensor bit mapirane
static const int8_t s_sensor_map[HC165_TOTAL_BITS] = SENSOR_MAP_DEFAULT;

/* ────────────────────────────────────────────────────────────────────── */

esp_err_t sensors_init(void)
{
    gpio_config_t out_cfg = {
        .pin_bit_mask = (1ULL<< PIN_HC165_PL),
        .mode      = GPIO_MODE_OUTPUT,
        .pull_up_en  = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type   = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&out_cfg));

    gpio_config_t in_cfg = {
        .pin_bit_mask = (1ULL << PIN_HC165_QH),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   =GPIO_PULLUP_ENABLE,  
        .pull_down_en= GPIO_PULLDOWN_DISABLE,
        .intr_type= GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&in_cfg));

    
    gpio_config_t clk_cfg = {
        .pin_bit_mask = (1ULL << PIN_CLK_SHARED) | (1ULL << PIN_APA_DATA),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&clk_cfg));

    /* Safe initial states */
    _SET(PIN_HC165_PL);  
    _CLR(PIN_CLK_SHARED); 
    _CLR(PIN_APA_DATA);   

    ESP_LOGI(TAG, "HC165 sensor chain ready (%d bits)", HC165_TOTAL_BITS);
    return ESP_OK;
}


void sensors_read(uint64_t *occupied)
{
    uint64_t raw = 0;


    _CLR(PIN_APA_DATA);

    // Povlekuvanje na data od site senzori
    _CLR(PIN_HC165_PL);   
    ets_delay_us(HC165_LATCH_US);
    _SET(PIN_HC165_PL);     
    ets_delay_us(1); 

    for (int i = 0; i < HC165_TOTAL_BITS; i++) {
        /* Read current Q7 */
        if (_RD(PIN_HC165_QH)) {
            raw |= (1ULL << i);
        }

        /* Clock next bit (skip extra clock after last bit) */
        if (i < HC165_TOTAL_BITS - 1) {
            _SET(PIN_CLK_SHARED);
            ets_delay_us(BB_HALF_CYCLE_US);
            _CLR(PIN_CLK_SHARED);
            ets_delay_us(BB_HALF_CYCLE_US);
        }
    }


    _CLR(PIN_CLK_SHARED);


    uint64_t occ = 0;
    for (int i = 0; i < HC165_TOTAL_BITS; i++) {
        if (!(raw & (1ULL << i))) {   
            int sq = s_sensor_map[i];
            if (sq >= 0 && sq < 64) {
                occ |= (1ULL << sq);
            }
        }
    }

    *occupied = occ;
}
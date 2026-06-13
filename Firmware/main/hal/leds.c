/*
 * hal/leds.c — APA102C driver, software SPI, CLK shared with HC165
 */

#include "leds.h"
#include "config.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "soc/gpio_reg.h"
#include "rom/ets_sys.h"
#include <string.h>

static const char *TAG = "leds";

/* ── Fast register-level GPIO ────────────────────────────────────────── */
#define _SET(pin)  REG_WRITE(GPIO_OUT_W1TS_REG, 1UL << (pin))
#define _CLR(pin)  REG_WRITE(GPIO_OUT_W1TC_REG, 1UL << (pin))

/* ── Internal: clock one bit MSB-first ───────────────────────────────── */
static inline void __attribute__((always_inline)) send_bit(uint8_t bit)
{
    if (bit) { _SET(PIN_APA_DATA); } else { _CLR(PIN_APA_DATA); }
    ets_delay_us(BB_HALF_CYCLE_US);
    _SET(PIN_CLK_SHARED);
    ets_delay_us(BB_HALF_CYCLE_US);
    _CLR(PIN_CLK_SHARED);
}

static void send_byte(uint8_t b)
{
    for (int i = 7; i >= 0; i--) {
        send_bit((b >> i) & 1);
    }
}

static void send_u32(uint32_t w)
{
    send_byte((w >> 24) & 0xFF);
    send_byte((w >> 16) & 0xFF);
    send_byte((w >>  8) & 0xFF);
    send_byte((w      ) & 0xFF);
}

/* ────────────────────────────────────────────────────────────────────── */
/* Forward declaration — leds_write is defined below leds_init */
void leds_write(const rgb_t leds[64]);

esp_err_t leds_init(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << PIN_APA_DATA) | (1ULL << PIN_CLK_SHARED),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));

    _CLR(PIN_APA_DATA);
    _CLR(PIN_CLK_SHARED);

    /* Boot: blank all LEDs */
    rgb_t blank[64];
    memset(blank, 0, sizeof(blank));
    leds_write(blank);

    ESP_LOGI(TAG, "APA102C chain ready (%d LEDs)", LED_COUNT);
    return ESP_OK;
}

/* ────────────────────────────────────────────────────────────────────── */

void leds_write(const rgb_t leds[64])
{
    /* ── Start frame: 32 zero bits ──────────────────────────────────── */
    send_u32(0x00000000);

    /* ── LED frames ─────────────────────────────────────────────────── */
    /*
     * Frame layout (32 bits per LED):
     *   Bits 31-24: 0b111ggggg  (3 ones + 5-bit global brightness)
     *   Bits 23-16: Blue
     *   Bits 15- 8: Green
     *   Bits  7- 0: Red
     *
     * LED index 0 = A8, index 63 = H1 (matches leds[] parameter order).
     */
    for (int i = 0; i < LED_COUNT; i++) {
        uint8_t header = 0xE0 | (LED_GLOBAL_BRIGHT & 0x1F);
        send_byte(header);
        send_byte(leds[i].b);   /* Blue  first */
        send_byte(leds[i].g);
        send_byte(leds[i].r);
    }

    /*
     * ── End frame ────────────────────────────────────────────────────
     * APA102C requires ≥ ceil(N/2) additional clock pulses after the
     * last LED frame to propagate the data through the chain.
     * For N=64: ceil(64/2) = 32 bits = 4 bytes of 0xFF.
     * Sending 64 extra bits (8 bytes) is safe overkill.
     */
    send_u32(0xFFFFFFFF);
    send_u32(0xFFFFFFFF);

    /* Return CLK and DATA to idle LOW */
    _CLR(PIN_CLK_SHARED);
    _CLR(PIN_APA_DATA);
}

/* ────────────────────────────────────────────────────────────────────── */

void leds_clear(void)
{
    rgb_t blank[64];
    memset(blank, 0, sizeof(blank));
    leds_write(blank);
}

void leds_fill(rgb_t buf[64], rgb_t color)
{
    for (int i = 0; i < 64; i++) {
        buf[i] = color;
    }
}

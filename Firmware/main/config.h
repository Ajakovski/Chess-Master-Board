#pragma once //code reference has been taken from official library sources in order to define

#include "driver/gpio.h"

// GPIO
#define PIN_BATTERY_ADC     GPIO_NUM_1
#define PIN_BTN_P1          GPIO_NUM_6
#define PIN_BTN_P2          GPIO_NUM_7
#define PIN_I2C_SDA         GPIO_NUM_8
#define PIN_I2C_SCL         GPIO_NUM_9
#define PIN_HC165_PL        GPIO_NUM_10
#define PIN_APA_DATA        GPIO_NUM_11
#define PIN_CLK_SHARED      GPIO_NUM_12
#define PIN_HC165_QH        GPIO_NUM_13


#define BB_HALF_CYCLE_US    1

//shift-register
#define HC165_NUM_CHIPS     8
#define HC165_TOTAL_BITS    64
#define HC165_LATCH_US      2

#define SENSOR_MAP_DEFAULT                                  \
{                                                           \
    56,57,58,59,60,61,62,63,                               \
    48,49,50,51,52,53,54,55,                               \
    40,41,42,43,44,45,46,47,                               \
    32,33,34,35,36,37,38,39,                               \
    24,25,26,27,28,29,30,31,                               \
    16,17,18,19,20,21,22,23,                               \
     8, 9,10,11,12,13,14,15,                               \
     0, 1, 2, 3, 4, 5, 6, 7                                \
}

//LED
#define LED_COUNT           64
#define LED_GLOBAL_BRIGHT   0x08

/*LED COLOURS
*/
#define LED_COLOR_OFF           {  0,   0,   0}
#define LED_COLOR_MOVE_FROM     {255, 140,   0}
#define LED_COLOR_MOVE_TO       {  0, 220,   0}
#define LED_COLOR_SF_FROM       {  0,  80, 255}
#define LED_COLOR_SF_TO         {  0, 160, 255}
#define LED_COLOR_VALID_DST     { 80,  80,   0}
#define LED_COLOR_INVALID       {255,   0,   0}
#define LED_COLOR_CHECK         {255,   0, 200}
#define LED_COLOR_CAPTURE_SRC   {255,  60,   0}
#define LED_COLOR_CAPTURE_DST   {255, 100,   0}
#define LED_COLOR_CONFIRM_FROM  {255, 200,   0}
#define LED_COLOR_CONFIRM_TO    {  0, 255, 100}

/* I2c oled*/
#define I2C_PORT            I2C_NUM_0
#define I2C_FREQ_HZ         400000
#define OLED_I2C_ADDR       0x3C
#define OLED_WIDTH          128
#define OLED_HEIGHT         64
#define OLED_PAGES          (OLED_HEIGHT / 8)


#define BATTERY_R1_KOHM         100.0f
#define BATTERY_R2_KOHM         100.0f
#define BATTERY_DIV             (BATTERY_R2_KOHM / (BATTERY_R1_KOHM + BATTERY_R2_KOHM))
#define BATTERY_VMAX_MV         4200
#define BATTERY_VMIN_MV         3000
#define BATTERY_ADC_CHANNEL     ADC_CHANNEL_0
#define BATTERY_ADC_ATTEN       ADC_ATTEN_DB_12
#define BATTERY_SAMPLES         16
#define BATTERY_POLL_MS         30000

/*button*/
#define BTN_DEBOUNCE_MS         20
#define BTN_SHORT_MAX_MS        1500
#define SLEEP_HOLD_BOTH_MS      5000

/* GAME*/
#define GAME_CLOCK_MS           (10 * 60 * 1000)
#define CLOCK_TICK_MS           100
#define BOARD_STABLE_MS         300
#define BOARD_POLL_MS           10

#define SF_SEARCH_DEPTH         8
#define MCU_MAX_HASH_KB         512
#define MCU_MAX_TASK_STACK      (12 * 1024)


#define SF_CMD_BUF_SIZE         256
#define SF_REPLY_BUF_SIZE       512
#define SF_QUEUE_LEN            16
#define SF_MOVE_TIMEOUT_MS      15000

// deep sleep
#define RTC_MAGIC               0xCB3D1A7EU
#define MAX_HALF_MOVES          500

// indekuvanje na kvadrati
#define SQ(file, rank)          ((rank) * 8 + (file))
#define SQ_FILE(sq)             ((sq) % 8)
#define SQ_RANK(sq)             ((sq) / 8)
#define SQ_VALID(sq)            ((sq) >= 0 && (sq) < 64)
#define SQ_LED(sq)              ((7 - SQ_RANK(sq)) * 8 + SQ_FILE(sq))
#define SQ_NONE                 (-1)

#define SQ_A1  SQ(0,0)
#define SQ_E1  SQ(4,0)
#define SQ_G1  SQ(6,0)
#define SQ_H1  SQ(7,0)
#define SQ_A8  SQ(0,7)
#define SQ_E8  SQ(4,7)
#define SQ_G8  SQ(6,7)
#define SQ_H8  SQ(7,7)

// Freertos poredeni naredbi
#define TASK_PRIO_BUTTON        7
#define TASK_PRIO_SENSOR        6
#define TASK_PRIO_LED           6
#define TASK_PRIO_GAME          5
#define TASK_PRIO_DISPLAY       3
#define TASK_PRIO_STOCKFISH     2
#define TASK_PRIO_BATTERY       1

#define TASK_STACK_BUTTON       2048
#define TASK_STACK_SENSOR       3072
#define TASK_STACK_LED          3072
#define TASK_STACK_GAME         8192
#define TASK_STACK_DISPLAY      4096
#define TASK_STACK_BATTERY      2048

#define CORE_GAME               0
#define CORE_STOCKFISH          1
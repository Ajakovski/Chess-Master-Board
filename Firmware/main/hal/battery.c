#include "battery.h"
#include "config.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>

static const char *TAG = "battery";

static adc_oneshot_unit_handle_t s_adc_handle;
static adc_cali_handle_t         s_cali_handle;
static bool                      s_cali_valid = false;
static volatile uint8_t          s_percent    = 100;

/* Moving average ring buffer */
static int32_t s_samples[BATTERY_SAMPLES];
static int     s_idx    = 0;
static bool    s_filled = false;

/*One calibrated voltage reading (mV)*/
static int32_t read_mv(void)
{
    int raw = 0;
    if (adc_oneshot_read(s_adc_handle, BATTERY_ADC_CHANNEL, &raw) != ESP_OK) {
        return -1;
    }

    int mv_adc = 0;
    if (s_cali_valid) {
        adc_cali_raw_to_voltage(s_cali_handle, raw, &mv_adc);
    } else {
        mv_adc = (int32_t)((int64_t)raw * 3550 / 4095);
    }

    return (int32_t)(mv_adc * (BATTERY_R1_KOHM + BATTERY_R2_KOHM) / BATTERY_R2_KOHM);
}

/*Convert mV to percentage*/
static uint8_t mv_to_pct(int32_t mv)
{
    if (mv >= BATTERY_VMAX_MV) return 100;
    if (mv <= BATTERY_VMIN_MV) return 0;
    return (uint8_t)(((int32_t)(mv - BATTERY_VMIN_MV) * 100)
                     / (BATTERY_VMAX_MV - BATTERY_VMIN_MV));
}

// Avarage od samlping
static int32_t moving_avg_mv(int32_t new_mv)
{
    s_samples[s_idx] = new_mv;
    s_idx = (s_idx + 1) % BATTERY_SAMPLES;
    if (s_idx == 0) s_filled = true;

    int count = s_filled ? BATTERY_SAMPLES : s_idx;
    int64_t sum = 0;
    for (int i = 0; i < count; i++) sum += s_samples[i];
    return (int32_t)(sum / count);
}



esp_err_t battery_init(void)
{
    /* ADC unit */
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id  = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &s_adc_handle));

    /* Channel config */
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten    = BATTERY_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc_handle,
                                               BATTERY_ADC_CHANNEL, &chan_cfg));

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    adc_cali_curve_fitting_config_t cali_cfg = {
        .unit_id  = ADC_UNIT_1,
        .atten    = BATTERY_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_12,
    };
    s_cali_valid = (adc_cali_create_scheme_curve_fitting(&cali_cfg,
                                                          &s_cali_handle) == ESP_OK);
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    adc_cali_line_fitting_config_t cali_cfg = {
        .unit_id  = ADC_UNIT_1,
        .atten    = BATTERY_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_12,
    };
    s_cali_valid = (adc_cali_create_scheme_line_fitting(&cali_cfg,
                                                         &s_cali_handle) == ESP_OK);
#endif

    if (!s_cali_valid) {
        ESP_LOGW(TAG, "ADC calibration unavailable — using raw approximation");
    }

    /* fill the moving-average buffer */
    for (int i = 0; i < BATTERY_SAMPLES; i++) {
        int32_t mv = read_mv();
        if (mv > 0) moving_avg_mv(mv);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    int32_t init_mv = moving_avg_mv(read_mv());
    s_percent = mv_to_pct(init_mv);
    ESP_LOGI(TAG, "Battery: %ld mV → %d%%", init_mv, s_percent);
    return ESP_OK;
}



uint8_t battery_percent(void) { return s_percent; }

void battery_task(void *arg)
{
    (void)arg;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(BATTERY_POLL_MS));
        int32_t mv  = read_mv();
        if (mv > 0) {
            int32_t avg = moving_avg_mv(mv);
            s_percent   = mv_to_pct(avg);
            ESP_LOGD(TAG, "Battery: %ld mV → %d%%", avg, s_percent);
        }
    }
}
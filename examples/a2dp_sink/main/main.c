// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"

#include "esp_bt.h"
#include "bt_app_core.h"
#include "bt_app_av.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"
#include "driver/i2s.h"

#include "screen_driver.h"
#include "mic.h"
#include "esp_timer.h"

static const char *TAG = "bt fft";

/* event for handler "bt_av_hdl_stack_up */
enum {
    BT_APP_EVT_STACK_UP = 0,
};

/* handler for bluetooth stack enabled events */
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);
static scr_driver_t g_lcd;

static void screen_clear(scr_driver_t *lcd, int color)
{
    scr_info_t lcd_info;
    lcd->get_info(&lcd_info);

    uint16_t *buffer = malloc(lcd_info.width * sizeof(uint16_t));
    if (NULL == buffer) {
        for (size_t y = 0; y < lcd_info.height; y++) {
            for (size_t x = 0; x < lcd_info.width; x++) {
                lcd->draw_pixel(x, y, color);
            }
        }
    } else {
        for (size_t i = 0; i < lcd_info.width; i++) {
            buffer[i] = color;
        }

        for (int y = 0; y < lcd_info.height; y++) {
            lcd->draw_bitmap(0, y, lcd_info.width, 1, buffer);
        }

        free(buffer);
    }
}

static void _lcd_init()
{
    spi_config_t spi_cfg = {
        .miso_io_num = -1,
        .mosi_io_num = 21,
        .sclk_io_num = 22,
        .max_transfer_sz = 240 * 240 * 2 + 10,
    };
    spi_bus_handle_t spi_bus = spi_bus_create(2, &spi_cfg);
    // TEST_ASSERT_NOT_NULL(spi_bus);

    scr_interface_spi_config_t spi_lcd_cfg = {
        .spi_bus = spi_bus,
        .pin_num_cs = 5,
        .pin_num_dc = 19,
        .clk_freq = 80000000,
        .swap_data = false,
    };

    scr_interface_driver_t *iface_drv;
    if (ESP_OK == scr_interface_create(SCREEN_IFACE_SPI, &spi_lcd_cfg, &iface_drv)) {
    }

    scr_controller_config_t lcd_cfg = {0};
    lcd_cfg.interface_drv = iface_drv;
    lcd_cfg.pin_num_rst = -1;
    lcd_cfg.pin_num_bckl = 23;
    lcd_cfg.rst_active_level = 0;
    lcd_cfg.bckl_active_level = 1;
    lcd_cfg.offset_hor = 0;
    lcd_cfg.offset_ver = 0;
    lcd_cfg.width = 240;
    lcd_cfg.height = 120;
    lcd_cfg.rotate = SCR_DIR_LRBT;
    if (ESP_OK == scr_find_driver(SCREEN_CONTROLLER_ST7789, &g_lcd)) {
    }
    if (ESP_OK == g_lcd.init(&lcd_cfg)) {
    }
    screen_clear(&g_lcd, COLOR_WHITE);
}

#include <math.h>
#include "esp_dsp.h"
#define N_SAMPLES 1024
#define N N_SAMPLES
#define CONFIG_DSP_MAX_FFT_SIZE 4096
#define TAG BT_APP_CORE_TAG

// Window coefficients
__attribute__((aligned(16))) static float wind[N_SAMPLES];
// working complex array
__attribute__((aligned(16))) static float y_cf[N_SAMPLES * 2];
static uint64_t g_period=0;

static void _fft_init()
{
    esp_err_t ret;
    ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    // Generate hann window
    dsps_wind_hann_f32(wind, N);
}

void _fft_data_input(void *data, size_t data_size)
{
    static size_t received_len = 0;
    data_size /= 4;
    int16_t *ap = (int16_t *)data;
    size_t read_len = 0;
    while (data_size > 0) {
        size_t empty = N - received_len;
        if (empty > data_size) {
            for (int i = 0; i < data_size; i++) {
                y_cf[(received_len + i) * 2 + 0] = (float)ap[(read_len + i) * 2] / 32768.0;
                // y_cf[(received_len+i)*2+1] = (float)ap[(read_len+i)*2+1]/32768.0;
            }
            received_len += data_size;
            data_size -= data_size;
            read_len += data_size;
            return;
        } else {
            for (int i = 0; i < empty; i++) {
                y_cf[(received_len + i) * 2 + 0] = (float)ap[(read_len + i) * 2] / 32768.0;
                // y_cf[(received_len+i)*2+1] = (float)ap[(read_len+i)*2+1]/32768.0;
            }
            received_len += empty;
            data_size -= empty;
            read_len += empty;
        }

        if (received_len != N) {
            ESP_LOGW(TAG, "len err");
        }
        received_len = 0;

        static int cnt = 0;
        if (++cnt > 2) {
            cnt = 0;
            // hanming window
            for (int i = 0; i < N; i++) {
                y_cf[i * 2 + 0] *= wind[i];
                y_cf[i * 2 + 1] = 0; //= wind[i];
            }
            // FFT
            dsps_fft2r_fc32(y_cf, N);
            // Bit reverse
            dsps_bit_rev_fc32(y_cf, N);
            // Convert one complex vector to two complex vectors
            dsps_cplx2reC_fc32(y_cf, N);
            // Pointers to result arrays
            float *y1_cf = &y_cf[0];
            float *y2_cf = &y_cf[N_SAMPLES];

            for (int i = 0; i < N / 2; i++) {
                float a = y1_cf[i * 2 + 0];
                float b = y1_cf[i * 2 + 1];
                y1_cf[i] = 10 * log10f((a * a + b * b) / N);
                // y2_cf[i] = 10 * log10f((y2_cf[i * 2 + 0] * y2_cf[i * 2 + 0] + y2_cf[i * 2 + 1] * y2_cf[i * 2 + 1])/N);
            }
            // Show power spectrum in 64x10 window from -100 to 0 dB from 0..N/4 samples
            // ESP_LOGW(TAG, "Signal x1");
            // dsps_view(y1_cf, N/2, 64, 10,  -60, 40, '*');
            const int H = 50;
            uint16_t disp_buf[H];
            static uint64_t last_time=0;
            uint64_t t = esp_timer_get_time();
            g_period = t - last_time;
            last_time = t;
            for (size_t x = 0; x < 230; x++) {
                float t = y1_cf[x * 2] + 40;
                if (t < 0.0) {
                    t = 0;
                }

                for (size_t y = 0; y < H; y++) {
                    if (t >= y) {
                        disp_buf[y] = 0x00F8;
                    } else {
                        disp_buf[y] = COLOR_WHITE;
                    }
                }
                g_lcd.draw_bitmap(5 + x, 0, 1, H, disp_buf);
            }
        }
    }
}
// mic_get_data(buffer, 16*16000*40/1000, &len, portMAX_DELAY);
// int16_t *pcm = (int16_t *)buffer;
// for (size_t i = 0; i < len / 2; i++) {
//     pcm[i] = pcm[i] >> 8 | pcm[i]<<8;
// }

void app_main(void)
{
    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    _fft_init();
    _lcd_init();
    // mic_init(I2S_NUM_1);

    i2s_config_t i2s_config = {
#ifdef CONFIG_EXAMPLE_A2DP_SINK_OUTPUT_INTERNAL_DAC
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
#else
        .mode = I2S_MODE_MASTER | I2S_MODE_TX, // Only TX
#endif
        .sample_rate = 44100,
        .bits_per_sample = 16,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, //2-channels
        .communication_format = I2S_COMM_FORMAT_STAND_MSB,
        .dma_buf_count = 6,
        .dma_buf_len = 60,
        .intr_alloc_flags = 0,     //Default interrupt priority
        .tx_desc_auto_clear = true //Auto clear tx descriptor on underflow
    };

    i2s_driver_install(0, &i2s_config, 0, NULL);
#ifdef CONFIG_EXAMPLE_A2DP_SINK_OUTPUT_INTERNAL_DAC
    i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
    i2s_set_pin(0, NULL);
#else
    i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_EXAMPLE_I2S_BCK_PIN,
        .ws_io_num = CONFIG_EXAMPLE_I2S_LRCK_PIN,
        .data_out_num = CONFIG_EXAMPLE_I2S_DATA_PIN,
        .data_in_num = -1 //Not used
    };

    i2s_set_pin(0, &pin_config);
#endif

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((err = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    if ((err = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(BT_AV_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(err));
        return;
    }

    /* create application task */
    bt_app_task_start_up();

    /* Bluetooth device name, connection mode and profile set up */
    bt_app_work_dispatch(bt_av_hdl_stack_evt, BT_APP_EVT_STACK_UP, NULL, 0, NULL);

#if (CONFIG_BT_SSP_ENABLED == true)
    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
#endif

    /*
     * Set default parameters for Legacy Pairing
     * Use fixed pin code
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code;
    pin_code[0] = '1';
    pin_code[1] = '2';
    pin_code[2] = '3';
    pin_code[3] = '4';
    esp_bt_gap_set_pin(pin_type, 4, pin_code);
    while (1)
    {
        ESP_LOGW(TAG, "fps=%.2f\n", 1000.0f/(float)(g_period/1000));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}

void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
    case ESP_BT_GAP_AUTH_CMPL_EVT: {
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
            ESP_LOGI(BT_AV_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            esp_log_buffer_hex(BT_AV_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
        } else {
            ESP_LOGE(BT_AV_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }

#if (CONFIG_BT_SSP_ENABLED == true)
    case ESP_BT_GAP_CFM_REQ_EVT:
        ESP_LOGI(BT_AV_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", param->cfm_req.num_val);
        esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        ESP_LOGI(BT_AV_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
        break;
    case ESP_BT_GAP_KEY_REQ_EVT:
        ESP_LOGI(BT_AV_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;
#endif

    case ESP_BT_GAP_MODE_CHG_EVT:
        ESP_LOGI(BT_AV_TAG, "ESP_BT_GAP_MODE_CHG_EVT mode:%d", param->mode_chg.mode);
        break;

    default: {
        ESP_LOGI(BT_AV_TAG, "event: %d", event);
        break;
    }
    }
    return;
}
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param)
{
    ESP_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    switch (event) {
    case BT_APP_EVT_STACK_UP: {
        /* set up device name */
        char *dev_name = "ESP_SPEAKER";
        esp_bt_dev_set_device_name(dev_name);

        esp_bt_gap_register_callback(bt_app_gap_cb);

        /* initialize AVRCP controller */
        esp_avrc_ct_init();
        esp_avrc_ct_register_callback(bt_app_rc_ct_cb);
        /* initialize AVRCP target */
        assert(esp_avrc_tg_init() == ESP_OK);
        esp_avrc_tg_register_callback(bt_app_rc_tg_cb);

        esp_avrc_rn_evt_cap_mask_t evt_set = {0};
        esp_avrc_rn_evt_bit_mask_operation(ESP_AVRC_BIT_MASK_OP_SET, &evt_set, ESP_AVRC_RN_VOLUME_CHANGE);
        assert(esp_avrc_tg_set_rn_evt_cap(&evt_set) == ESP_OK);

        /* initialize A2DP sink */
        esp_a2d_register_callback(&bt_app_a2d_cb);
        esp_a2d_sink_register_data_callback(bt_app_a2d_data_cb);
        esp_a2d_sink_init();

        /* set discoverable and connectable mode, wait to be connected */
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        break;
    }
    default:
        ESP_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "freertos/xtensa_api.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "bt_app_core.h"
#include "driver/i2s.h"
#include "freertos/ringbuf.h"

static void bt_app_task_handler(void *arg);
static bool bt_app_send_msg(bt_app_msg_t *msg);
static void bt_app_work_dispatched(bt_app_msg_t *msg);

static xQueueHandle s_bt_app_task_queue = NULL;
static xTaskHandle s_bt_app_task_handle = NULL;
static xTaskHandle s_bt_i2s_task_handle = NULL;
static RingbufHandle_t s_ringbuf_i2s = NULL;;

bool bt_app_work_dispatch(bt_app_cb_t p_cback, uint16_t event, void *p_params, int param_len, bt_app_copy_cb_t p_copy_cback)
{
    ESP_LOGD(BT_APP_CORE_TAG, "%s event 0x%x, param len %d", __func__, event, param_len);

    bt_app_msg_t msg;
    memset(&msg, 0, sizeof(bt_app_msg_t));

    msg.sig = BT_APP_SIG_WORK_DISPATCH;
    msg.event = event;
    msg.cb = p_cback;

    if (param_len == 0) {
        return bt_app_send_msg(&msg);
    } else if (p_params && param_len > 0) {
        if ((msg.param = malloc(param_len)) != NULL) {
            memcpy(msg.param, p_params, param_len);
            /* check if caller has provided a copy callback to do the deep copy */
            if (p_copy_cback) {
                p_copy_cback(&msg, msg.param, p_params);
            }
            return bt_app_send_msg(&msg);
        }
    }

    return false;
}

static bool bt_app_send_msg(bt_app_msg_t *msg)
{
    if (msg == NULL) {
        return false;
    }

    if (xQueueSend(s_bt_app_task_queue, msg, 10 / portTICK_RATE_MS) != pdTRUE) {
        ESP_LOGE(BT_APP_CORE_TAG, "%s xQueue send failed", __func__);
        return false;
    }
    return true;
}

static void bt_app_work_dispatched(bt_app_msg_t *msg)
{
    if (msg->cb) {
        msg->cb(msg->event, msg->param);
    }
}

static void bt_app_task_handler(void *arg)
{
    bt_app_msg_t msg;
    for (;;) {
        if (pdTRUE == xQueueReceive(s_bt_app_task_queue, &msg, (portTickType)portMAX_DELAY)) {
            ESP_LOGD(BT_APP_CORE_TAG, "%s, sig 0x%x, 0x%x", __func__, msg.sig, msg.event);
            switch (msg.sig) {
            case BT_APP_SIG_WORK_DISPATCH:
                bt_app_work_dispatched(&msg);
                break;
            default:
                ESP_LOGW(BT_APP_CORE_TAG, "%s, unhandled sig: %d", __func__, msg.sig);
                break;
            } // switch (msg.sig)

            if (msg.param) {
                free(msg.param);
            }
        }
    }
}

void bt_app_task_start_up(void)
{
    s_bt_app_task_queue = xQueueCreate(10, sizeof(bt_app_msg_t));
    xTaskCreate(bt_app_task_handler, "BtAppT", 3072, NULL, configMAX_PRIORITIES - 3, &s_bt_app_task_handle);
    return;
}

void bt_app_task_shut_down(void)
{
    if (s_bt_app_task_handle) {
        vTaskDelete(s_bt_app_task_handle);
        s_bt_app_task_handle = NULL;
    }
    if (s_bt_app_task_queue) {
        vQueueDelete(s_bt_app_task_queue);
        s_bt_app_task_queue = NULL;
    }
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
__attribute__((aligned(16))) static float y_cf[N_SAMPLES*2];
// Pointers to result arrays
static float* y1_cf = &y_cf[0];
static float* y2_cf = &y_cf[N_SAMPLES];

static void _fft_init()
{
    esp_err_t ret;
    ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }

    // Generate hann window
    dsps_wind_hann_f32(wind, N);
}

static void _fft(void *data, size_t data_size)
{
    static size_t received_len=0;
    data_size/=4;
    int16_t *ap = (int16_t*)data;
    size_t read_len=0;
    while (data_size>0)
    {
        size_t empty=N-received_len;
        if (empty > data_size)
        {
            for (int i=0 ; i< data_size ; i++)
            {
                y_cf[(received_len+i)*2+0] = (float)ap[(read_len+i)*2]/32768.0;
                y_cf[(received_len+i)*2+1] = (float)ap[(read_len+i)*2+1]/32768.0;
            }
            received_len+=data_size;
            data_size-=data_size;
            read_len+=data_size;
            return;
        }else{
            for (int i=0 ; i< empty ; i++)
            {
                y_cf[(received_len+i)*2+0] = (float)ap[(read_len+i)*2]/32768.0;
                y_cf[(received_len+i)*2+1] = (float)ap[(read_len+i)*2+1]/32768.0;
            }
            received_len+=empty;
            data_size-=empty;
            read_len+=empty;
        }
        
        if (received_len!=N)
        {
            ESP_LOGW(TAG, "len err");
        }
        received_len=0;
        
        for (int i=0 ; i< N ; i++)
        {
            y_cf[i*2 + 0] *= wind[i];
            y_cf[i*2 + 1] *= wind[i];
        }
        // FFT
        unsigned int start_b = dsp_get_cpu_cycle_count();
        dsps_fft2r_fc32(y_cf, N);
        unsigned int end_b = dsp_get_cpu_cycle_count();
        // Bit reverse 
        dsps_bit_rev_fc32(y_cf, N);
        // Convert one complex vector to two complex vectors
        dsps_cplx2reC_fc32(y_cf, N);

        static int cnt=0;
        if (++cnt>10)
        {
            cnt=0;
            for (int i = 0 ; i < N/2 ; i++) {
                y1_cf[i] = 10 * log10f((y1_cf[i * 2 + 0] * y1_cf[i * 2 + 0] + y1_cf[i * 2 + 1] * y1_cf[i * 2 + 1])/N);
                y2_cf[i] = 10 * log10f((y2_cf[i * 2 + 0] * y2_cf[i * 2 + 0] + y2_cf[i * 2 + 1] * y2_cf[i * 2 + 1])/N);
            }
            // Show power spectrum in 64x10 window from -100 to 0 dB from 0..N/4 samples
            // ESP_LOGW(TAG, "Signal x1");
            dsps_view(y1_cf, N/2, 64, 10,  -60, 40, '*');
            // ESP_LOGW(TAG, "Signal x2");
            // dsps_view(y2_cf, N/2, 64, 10,  -60, 40, '|');
            // ESP_LOGI(TAG, "FFT for %i complex points take %i cycles", N, end_b - start_b);
        }
    }
}


static void bt_i2s_task_handler(void *arg)
{
    uint8_t *data = NULL;
    size_t item_size = 0;
    size_t bytes_written = 0;
    _fft_init();
    
    for (;;) {
        data = (uint8_t *)xRingbufferReceive(s_ringbuf_i2s, &item_size, (portTickType)portMAX_DELAY);
        if (item_size != 0){
            _fft(data, item_size);
            // i2s_write(0, data, item_size, &bytes_written, portMAX_DELAY);
            vRingbufferReturnItem(s_ringbuf_i2s,(void *)data);
        }
    }
}

void bt_i2s_task_start_up(void)
{
    s_ringbuf_i2s = xRingbufferCreate(8 * 1024, RINGBUF_TYPE_BYTEBUF);
    if(s_ringbuf_i2s == NULL){
        return;
    }

    xTaskCreate(bt_i2s_task_handler, "BtI2ST", 4096, NULL, configMAX_PRIORITIES - 3, &s_bt_i2s_task_handle);
    return;
}

void bt_i2s_task_shut_down(void)
{
    if (s_bt_i2s_task_handle) {
        vTaskDelete(s_bt_i2s_task_handle);
        s_bt_i2s_task_handle = NULL;
    }

    if (s_ringbuf_i2s) {
        vRingbufferDelete(s_ringbuf_i2s);
        s_ringbuf_i2s = NULL;
    }
}

size_t write_ringbuf(const uint8_t *data, size_t size)
{
    BaseType_t done = xRingbufferSend(s_ringbuf_i2s, (void *)data, size, (portTickType)portMAX_DELAY);
    if(done){
        return size;
    } else {
        return 0;
    }
}

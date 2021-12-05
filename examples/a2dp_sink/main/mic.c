
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/i2s.h"
#include "esp_log.h"

static const char *TAG = "mic";

#define BOARD_DMIC_I2S_SCK 26
#define BOARD_DMIC_I2S_WS 32
#define BOARD_DMIC_I2S_SDO 33

static i2s_port_t g_port = 0;

void mic_init(i2s_port_t i2s_port)
{
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX,           // the mode must be set according to DSP configuration
        .sample_rate = 32000,                            // must be the same as DSP configuration
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,    // must be the same as DSP configuration
        .bits_per_sample = 32,                           // must be the same as DSP configuration
        .communication_format = I2S_COMM_FORMAT_I2S,
        .dma_buf_count = 3,
        .dma_buf_len = 1024,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = BOARD_DMIC_I2S_SCK,  // IIS_SCLK
        .ws_io_num = BOARD_DMIC_I2S_WS,    // IIS_LCLK
        .data_out_num = -1,                // IIS_DSIN
        .data_in_num = BOARD_DMIC_I2S_SDO  // IIS_DOUT
    };
    i2s_driver_install(i2s_port, &i2s_config, 0, NULL);
    i2s_set_pin(i2s_port, &pin_config);
    i2s_zero_dma_buffer(i2s_port);
    g_port = i2s_port;
}

void mic_get_data(uint8_t *in_buffer, size_t size, size_t *bytes_read, TickType_t ticks_to_wait)
{
    uint32_t *buffer = (uint32_t *)in_buffer;
    i2s_read(g_port, in_buffer, size, bytes_read, ticks_to_wait);

    // *bytes_read /= 2;
    int len = *bytes_read/4/4;
    for (int x = 0; x < len; x++) {
        int s1 = ((buffer[x*4]<<1) + 0) & 0xffff0000;
        int s2 = ((buffer[x*4+2]<<1) + 0) >> 16;
         buffer[x] = s1|s2;
    }

    *bytes_read /= 8;

// uint16_t *d = (uint16_t*)buffer;
//     printf("[");
//     for (size_t i = 0; i < 8; i++)
//     {
       
//         printf("%d, ", d[i]);
//     }printf("]%d\n", *bytes_read);
}



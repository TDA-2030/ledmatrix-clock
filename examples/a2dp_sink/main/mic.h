#ifndef _APP_MICROPHONE_H_
#define _APP_MICROPHONE_H_

#include "esp_system.h"
#include "driver/i2s.h"

#ifdef __cplusplus
extern "C" {
#endif

void mic_init(i2s_port_t i2s_port);
void mic_get_data(uint8_t *buffer, size_t size, size_t *bytes_read, TickType_t ticks_to_wait);

#ifdef __cplusplus
}
#endif

#endif
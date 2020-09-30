#ifndef __NVS_STORAGE__H
#define __NVS_STORAGE__H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"

void nvs_storage_init(void);
esp_err_t nvs_storage_save_wifi_config(wifi_config_t *wifi_config);
esp_err_t nvs_storage_read_wifi_config(wifi_config_t *wifi_config);


#endif
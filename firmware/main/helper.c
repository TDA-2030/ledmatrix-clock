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
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "nvs.h"
#include "esp32/rom/rtc.h"
#include "helper.h"

static const char *TAG = "helper";

#define HELPER_CHECK_GT(a, str, label) \
    if (!(a)) { \
        ESP_LOGE(TAG,"%s(%d): %s", __FUNCTION__, __LINE__, str); \
        goto label; \
    }

#define HELPER_CHECK(a, str, ret) \
    if (!(a)) { \
        ESP_LOGE(TAG,"%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return ret; \
    }

esp_err_t iot_param_save(const char *space_name, const char *key, void *param, uint16_t len)
{
    esp_err_t ret = ESP_ERR_INVALID_ARG;
    HELPER_CHECK_GT(NULL != space_name, "Pointer of space_name is invalid", OPEN_FAIL);
    HELPER_CHECK_GT(NULL != key, "Pointer of key is invalid", OPEN_FAIL);
    HELPER_CHECK_GT(NULL != param, "Pointer of param is invalid", OPEN_FAIL);
    nvs_handle_t my_handle;
    ret = nvs_open(space_name, NVS_READWRITE, &my_handle);
    HELPER_CHECK_GT(ESP_OK == ret, "nvs open failed", OPEN_FAIL);
    ret = nvs_set_blob(my_handle, key, param, len);
    HELPER_CHECK_GT(ESP_OK == ret, "nvs set blob failed", SAVE_FINISH);
    ret = nvs_commit(my_handle);

SAVE_FINISH:
    nvs_close(my_handle);

OPEN_FAIL:
    return ret;
}

esp_err_t iot_param_load(const char *space_name, const char *key, void *dest)
{
    esp_err_t ret = ESP_ERR_INVALID_ARG;
    HELPER_CHECK_GT(NULL != space_name, "Pointer of space_name is invalid", OPEN_FAIL);
    HELPER_CHECK_GT(NULL != key, "Pointer of key is invalid", OPEN_FAIL);
    HELPER_CHECK_GT(NULL != dest, "Pointer of dest is invalid", OPEN_FAIL);
    nvs_handle_t my_handle;
    size_t required_size = 0;
    ret = nvs_open(space_name, NVS_READWRITE, &my_handle);
    HELPER_CHECK_GT(ESP_OK == ret, "nvs open failed", OPEN_FAIL);
    ret = nvs_get_blob(my_handle, key, NULL, &required_size);
    HELPER_CHECK_GT(ESP_OK == ret, "nvs get blob failed", LOAD_FINISH);
    if (required_size == 0) {
        ESP_LOGW(TAG, "the target you want to load has never been saved");
        ret = ESP_FAIL;
        goto LOAD_FINISH;
    }
    ret = nvs_get_blob(my_handle, key, dest, &required_size);

LOAD_FINISH:
    nvs_close(my_handle);

OPEN_FAIL:
    return ret;
}

esp_err_t iot_param_erase(const char *space_name, const char *key)
{
    esp_err_t ret = ESP_ERR_INVALID_ARG;
    HELPER_CHECK_GT(NULL != space_name, "Pointer of space_name is invalid", OPEN_FAIL);
    HELPER_CHECK_GT(NULL != key, "Pointer of key is invalid", OPEN_FAIL);
    nvs_handle_t my_handle;
    ret = nvs_open(space_name, NVS_READWRITE, &my_handle);
    HELPER_CHECK_GT(ESP_OK == ret, "nvs open failed", OPEN_FAIL);
    ret = nvs_erase_key(my_handle, key);
    HELPER_CHECK_GT(ESP_OK == ret, "nvs erase failed", ERASE_FINISH);
    ret = nvs_commit(my_handle);

ERASE_FINISH:
    nvs_close(my_handle);

OPEN_FAIL:
    return ret;
}


#define DEVICE_MASTER_NETWORK_CONFIG_DURATION_MS    (60000)
#define DEVICE_RESTART_TIMEOUT_MS                   (3000)
#define DEVICE_STORE_RESTART_COUNT_NAME_SPACE       "RestartCntName"
#define DEVICE_STORE_RESTART_COUNT_KEY              "RestartCount"

static void restart_count_erase_timercb(void *timer)
{
    if (!xTimerStop(timer, portMAX_DELAY)) {
        ESP_LOGE(TAG, "xTimerStop timer: %p", timer);
    }

    if (!xTimerDelete(timer, portMAX_DELAY)) {
        ESP_LOGE(TAG, "xTimerDelete timer: %p", timer);
    }

    uint32_t restart_count = 1;
    iot_param_save(DEVICE_STORE_RESTART_COUNT_NAME_SPACE, DEVICE_STORE_RESTART_COUNT_KEY, &restart_count, sizeof(uint32_t));
    ESP_LOGD(TAG, "Reset restart count");
}

int restart_count_get()
{
    esp_err_t ret              = ESP_OK;
    static TimerHandle_t timer = NULL;
    uint32_t restart_count     = 0;
    RESET_REASON reset_reason  = rtc_get_reset_reason(0);
    ret = iot_param_load(DEVICE_STORE_RESTART_COUNT_NAME_SPACE, DEVICE_STORE_RESTART_COUNT_KEY, &restart_count);

    if (timer) {
        return restart_count;
    }

    /**< If the device restarts within the instruction time,
         the event_mdoe value will be incremented by one */
    if (reset_reason == POWERON_RESET || reset_reason == RTCWDT_RTC_RESET) {
        restart_count++;
        ESP_LOGD(TAG, "restart count: %d", restart_count);
    } else {
        restart_count = 1;
        ESP_LOGW(TAG, "restart reason: %d", reset_reason);
    }

    ret = iot_param_save(DEVICE_STORE_RESTART_COUNT_NAME_SPACE, DEVICE_STORE_RESTART_COUNT_KEY, &restart_count, sizeof(uint32_t));
    HELPER_CHECK(ret == ESP_OK, "Save the number of restarts failed", restart_count);

    timer = xTimerCreate("restart_count_erase", DEVICE_RESTART_TIMEOUT_MS / portTICK_RATE_MS,
                         false, NULL, restart_count_erase_timercb);
    HELPER_CHECK(NULL != timer, "xTimerCreate failed", restart_count);

    xTimerStart(timer, 0);

    return restart_count;
}


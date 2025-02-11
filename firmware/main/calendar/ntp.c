/* LwIP SNTP example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sntp.h"

static const char *TAG = "SNTP";

#define REF_TIME    1546300800 /* 01-Jan-2019 00:00:00 */

static void time_sync_notification_cb(struct timeval *tv);


static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    const char *ntp_servers[] = {
        "ntp.aliyun.com",
        "ntp.ntsc.ac.cn",
        "time.windows.com",
        "time.apple.com",
        "pool.ntp.org",
    };
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 1, 0)
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    for (size_t i = 0; i < sizeof(ntp_servers) / sizeof(char *); i++) {
        ESP_LOGI(TAG, "Setting NTP server %s", ntp_servers[i]);
        esp_sntp_setservername(i, ntp_servers[i]);
    }
    esp_sntp_init();
#else
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    for (size_t i = 0; i < sizeof(ntp_servers) / sizeof(char *); i++) {
        ESP_LOGI(TAG, "Setting NTP server %s", ntp_servers[i]);
        sntp_setservername(i, ntp_servers[i]);
    }
    sntp_init();
#endif
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);

}

bool _time_check(void)
{
    time_t now;
    time(&now);
    if (now > REF_TIME) {
        return true;
    }
    return false;
}

static esp_err_t esp_rmaker_print_current_time(void)
{
    if (_time_check() == false) {
        ESP_LOGI(TAG, "Time not synchronised yet.");
    }

    char strftime_buf[64];
    struct tm timeinfo;
    time_t now;
    // Set timezone to Eastern Standard Time and print local time
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);

    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

    return ESP_OK;
}

esp_err_t esp_rmaker_time_wait_for_sync(uint32_t ticks_to_wait)
{

    const uint32_t DEFAULT_TICKS = pdMS_TO_TICKS(2000); /* 2 seconds in ticks */
    ESP_LOGW(TAG, "Waiting for time to be synchronized. This may take time.");
    uint32_t ticks_remaining = ticks_to_wait;
    uint32_t ticks = DEFAULT_TICKS;
    while (ticks_remaining > 0) {
        if (_time_check() == true) {
            break;
        }
        ESP_LOGD(TAG, "Time not synchronized yet. Retrying...");
        ticks = ticks_remaining < DEFAULT_TICKS ? ticks_remaining : DEFAULT_TICKS;
        ticks_remaining -= ticks;
        vTaskDelay(ticks);
    }

    /* Check if ticks_to_wait expired and time is not synchronized yet. */
    if (_time_check() == false) {
        ESP_LOGE(TAG, "Time not synchronized within the provided ticks: %" PRIu32, ticks_to_wait);
        return ESP_FAIL;
    }

    return ESP_OK;
}

static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
    esp_rmaker_print_current_time();
}

esp_err_t sntp_start(void)
{
    ESP_LOGI(TAG, "Starting SNTP");
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 1, 0)
    if (esp_sntp_enabled()) {
#else
    if (sntp_enabled()) {
#endif
        ESP_LOGI(TAG, "SNTP already initialized.");
        return ESP_OK;
    }
    initialize_sntp();

    // esp_rmaker_time_wait_for_sync(portMAX_DELAY);


    return ESP_OK;
}

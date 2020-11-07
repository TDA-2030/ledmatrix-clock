

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "sht20.h"
#include "led_matrix.h"
#include "captive_portal.h"
#include "file_manage.h"
#include "file_server.h"
#include "weather.h"
#include "app_sntp.h"
#include "app_main.h"
#include "calendar.h"

static const char *TAG = "app_main";


SysStatus_t SystemStatus = 0;


uint16_t get_right_brightness(uint8_t hour)
{
    //自动控制显示亮度
    if (hour > 22 || hour < 6) { //深夜最暗
        return (15);
    } else if (hour > 17 || hour < 8) { //晚上和早上适当降低亮度
        return (40);
    } else {                   //白天最亮
        return (100);
    }
}

calendar_t *get_calendar_time(void)
{
    struct tm timeinfo;
    time_t now;

    static calendar_t cdr;
    time(&now);
    localtime_r(&now, &timeinfo);

    cdr.w_year = timeinfo.tm_year + 1900;
    cdr.w_month = timeinfo.tm_mon + 1; // 加 1 是因为 tm_mon 变量范围为 0~11
    cdr.w_date = timeinfo.tm_mday;
    cdr.week = timeinfo.tm_wday;

    cdr.hour = timeinfo.tm_hour;
    cdr.min = timeinfo.tm_min;
    cdr.sec = timeinfo.tm_sec;
    return &cdr;
}

void Display_task(void *pvParameter)
{
    LedMatrix_Clear();

    uint8_t dir = 0, x = 0, y = 0;
    while (1) {

        calendar_t *cdr = get_calendar_time();
        //if (time_last != 0)
        {

            LedMatrix_ShowNum(x, y, cdr->hour, 2, 16);
            LedMatrix_Fill(x + 17, y + 4, x + 18, y + 5, 1);
            LedMatrix_Fill(x + 17, y + 10, x + 18, y + 11, 1);
            LedMatrix_ShowNum(x + 20, y, cdr->min, 2, 16);
            LedMatrix_Fill(x + 37, y + 4, x + 38, y + 5, 1);
            LedMatrix_Fill(x + 37, y + 10, x + 38, y + 11, 1);
            LedMatrix_ShowNum(x + 40, y, cdr->sec, 2, 16);

            // LedMatrix_ShowNum(0, 16, sht20Info.humidity * 100, 4, 12);
            // LedMatrix_ShowNum(33, 16, sht20Info.tempreture * 100, 4, 12);

            LedMatrix_SetLight(get_right_brightness(cdr->hour));
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);

        if (0 == dir) {
            if (++y > 16) {
                dir = 1;
            }
        } else {
            if (--y < 1) {
                dir = 0;
            }
        }

    }
}

void net_handle_task(void *pvParameter)
{

    char str[128];

    while (1) {
        calendar_t *cdr = get_calendar_time();
        memset(str, 0, sizeof(str));
        calendar_get_lunar_str(cdr, str); printf("农历: %s\n", str);
        calendar_get_jieqi_str(cdr, str); printf("节气: %s\n", str);

        // weather_get("jian", WEATHER_TYPE_DAY);
        // weather_get("jian", WEATHER_TYPE_NOW);
        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    LedMatrix_init();
    vTaskDelay(500 / portTICK_PERIOD_MS);
    LedMatrix_SetLight(100);

    bool is_configured;
    captive_portal_start("ESP_WEB_CONFIG", NULL, &is_configured);

    if (is_configured) {
        wifi_config_t wifi_config;
        esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config);
        ESP_LOGI(TAG, "SSID:%s, PASSWORD:%s", wifi_config.sta.ssid, wifi_config.sta.password);
        ret = captive_portal_wait(30000 / portTICK_PERIOD_MS);
        if (ESP_OK != ret) {
            ESP_LOGE(TAG, "Connect to AP timeout, restart to entry configuration");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            nvs_flash_erase();
            esp_restart();
        }
    } else {
        ret = captive_portal_wait(600 * 1000 / portTICK_PERIOD_MS);
    }

    sntp_start();
    ESP_ERROR_CHECK(fm_init()); /* Initialize file storage */
    start_file_server();

    xTaskCreate(&Display_task, "Display_task", 1024 * 2, NULL, 6, NULL);
    xTaskCreate(&net_handle_task, "net_handle_task", 1024 * 6, NULL, 5, NULL);
}

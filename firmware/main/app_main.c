
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"

#include "board.h"
#include "helper.h"
#include "shtcx.h"
#include "led_matrix.h"
#include "tpl0401.h"
#include "captive_portal.h"
#include "adapt/esp32_wifi.h"
#include "file_manage.h"
#include "file_server.h"
#include "calendar/weather.h"
#include "calendar/ntp.h"
#include "calendar/calendar.h"
#include "app_main.h"
#include "mp3_player.h"
#include "lv_port.h"
#include "ui/ui.h"

static const char *TAG = "app_main";


SysStatus_t SystemStatus = 0;

static uint16_t get_right_brightness(uint8_t hour)
{
    uint16_t ret = 30;
    //自动控制显示亮度
    if (hour > 22 || hour < 6) { //深夜最暗
        ret = 20;
    } else if (hour > 17 || hour < 8) { //晚上和早上适当降低亮度
        ret = 35;
    } else {                   //白天最亮
        ret = 80;
    }
    return ret;
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


void update_display(void *pvParameter)
{
    char str[128];
    uint8_t step = 0;

    while (1) {
        calendar_t *cdr = get_calendar_time();

        switch (step) {
        case 0:
            show_text_stop();
            vTaskDelay(pdMS_TO_TICKS(20 * 1000));
            step = 1;
            break;
        case 1: {
            char jieqi[34];
            char lunar[34];
            memset(jieqi, 0, sizeof(jieqi));
            calendar_get_jieqi_str(cdr, jieqi);
            memset(lunar, 0, sizeof(lunar));
            calendar_get_lunar_str(cdr, lunar);
            memset(str, 0, sizeof(str));
            strcat(str, "今天是农历");
            strcat(str, lunar);
            strcat(str, "，");
            strcat(str, jieqi);
            ESP_LOGI(TAG, "%s", str);
            show_text_start(str);
            vTaskDelay(pdMS_TO_TICKS(20 * 1000));
            step = 0;
        }
        break;
        default:
            break;
        }

        LedMatrix_SetLight(get_right_brightness(cdr->hour));

        // {
        //     if (0)
        //     {
        //         float temperature=0, humidity=0;
        //         sht3cx_get_data(&humidity, &temperature);
        //         ESP_LOGI(TAG, "temp=%2.1f, humi=%2.1f", temperature, humidity);
        //         sprintf(str, "%2.1fC %2.0f%%", temperature, humidity);

        //     }

        //     vTaskDelay(25000 / portTICK_PERIOD_MS);
        // }


        // {
        //     memset(str, 0, sizeof(str));
        //     weather_get_str("shanghai", WEATHER_TYPE_NOW, str);

        //     paint_roll_text_create(2, 16, 60);
        //     paint_roll_text_register_done_cb(toll_done_cb);
        //     paint_roll_text_set_string(str, &Font16_gbk);
        //     xEventGroupWaitBits(xCreatedEventGroup, BIT0, pdTRUE, pdFALSE, portMAX_DELAY);
        //     paint_roll_text_delete();
        // }
        // {
        //     memset(str, 0, sizeof(str));
        //     weather_get_str("shanghai", WEATHER_TYPE_DAY, str);

        //     paint_roll_text_create(2, 16, 60);
        //     paint_roll_text_register_done_cb(toll_done_cb);
        //     paint_roll_text_set_string(str, &Font16_gbk);
        //     xEventGroupWaitBits(xCreatedEventGroup, BIT0, pdTRUE, pdFALSE, portMAX_DELAY);
        //     paint_roll_text_delete();
        // }

    }
}


static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == APP_NETWORK_EVENT) {
        switch (event_id) {
        case APP_NETWORK_EVENT_PROV_START:
            ESP_LOGI(TAG, "APP_NETWORK_EVENT_PROV_START");
            show_text_start("未联网，请进入192.168.4.1进行配网");
            break;
        case APP_NETWORK_EVENT_CONFIG_SUCCESS:
        case APP_NETWORK_EVENT_PROV_TIMEOUT:
            ESP_LOGI(TAG, "APP_NETWORK_EVENT_PROV_END");
            show_text_stop();
            break;
        default:
            break;
        }
    } else if (event_base == IP_EVENT) {
        switch (event_id) {
        case IP_EVENT_STA_GOT_IP:
            sntp_start();
            xTaskCreate(&update_display, "update_task", 1024 * 6, NULL, 5, NULL);
            break;
        case IP_EVENT_STA_LOST_IP:
            break;
        default:
            break;
        }
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
    board_init();

    const esp_partition_t *running = esp_ota_get_running_partition();
    ESP_LOGI(TAG, "Running partition: %s", running->label);

    /** Determine whether to restore the settings by reading the restart count */
    int restart_cnt = restart_count_get();
    ESP_LOGI(TAG, "Restart count=[%d]", restart_cnt);
    if (restart_cnt >= RESTART_COUNT_RESET) {
        ESP_LOGW(TAG, "Erase information saved in flash and restart");
        ESP_ERROR_CHECK(nvs_flash_erase());
        esp_restart();
    }
    ESP_ERROR_CHECK(fm_init()); /* Initialize file storage */
    LedMatrix_init();
    LedMatrix_SetLight(1);
    lv_port(LED_MATRIX_MAX_WIDTH, LED_MATRIX_MAX_HEIGHT);

    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (example_lvgl_lock(-1)) {
        init_lvgl_ui();
        // Release the mutex
        example_lvgl_unlock();
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
    LedMatrix_SetLight(20);

    bool is_configured;
    ESP_LOGI(TAG, "Setup Wifi ...");
    wifiIinitialize("时钟", "", &is_configured);
    ESP_ERROR_CHECK(esp_event_handler_register(APP_NETWORK_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    if (is_configured) {
        wifi_config_t wifi_config;
        esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config);
        char str[84];
        sprintf(str, "正在连接%s", wifi_config.sta.ssid);
        show_text_start(str);
        ESP_LOGI(TAG, "SSID:%s, PASSWORD:%s", wifi_config.sta.ssid, wifi_config.sta.password);
        xEventGroupWaitBits(g_wifi_event_group, WIFI_STA_GOT_IP, 0, 0, pdMS_TO_TICKS(10000));
        show_text_stop();
    } else {
        captive_portal_start();
    }


    // start_file_server();


    // mp3_player_init();
    // sht3cx_init();
    // tpl0401_init();
    // tpl0401_set_resistance(127);
    // mp3_player_start("/spiffs/01.mp3");
    // mp3_player_set_volume(0);
    // esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
}

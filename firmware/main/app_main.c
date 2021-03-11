
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "board.h"
#include "helper.h"
#include "shtcx.h"
#include "led_matrix.h"
#include "sh1106.h"
#include "ssd1306.h"
#include "captive_portal.h"
#include "file_manage.h"
#include "file_server.h"
#include "weather.h"
#include "app_sntp.h"
#include "app_main.h"
#include "calendar.h"
#include "mp3_player.h"
#include "paint.h"
#include "app_bt_audio.h"

static const char *TAG = "app_main";


SysStatus_t SystemStatus = 0;

uint16_t get_right_brightness(uint8_t hour)
{
    uint16_t ret = 30;
    //自动控制显示亮度
    if (hour > 22 || hour < 6) { //深夜最暗
        ret = 500;
    } else if (hour > 17 || hour < 8) { //晚上和早上适当降低亮度
        ret = 1200;
    } else {                   //白天最亮
        ret = 4095;
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

void Display_task(void *pvParameter)
{
    LedMatrix_Clear();
    iot_paint_Set_point_color(COLOR_GREEN);

    uint8_t x = 4, y = 0;

    paint_clock_init(x, y);
    calendar_t *cdr = get_calendar_time();
    while (1) {
        cdr = get_calendar_time();
        paint_clock_update(cdr->hour, cdr->min, cdr->sec);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

static EventGroupHandle_t xCreatedEventGroup;

static void toll_done_cb(void *arg)
{
    xEventGroupSetBits(xCreatedEventGroup, BIT0);
}

void net_handle_task(void *pvParameter)
{
    char str[128];
    sht3cx_init();

    xCreatedEventGroup = xEventGroupCreate();

    while (1) {
        calendar_t *cdr = get_calendar_time();
        LedMatrix_SetLight(get_right_brightness(cdr->hour));

        {
            static const char *Cdr_i2week[7] = { "天", "一", "二", "三", "四", "五", "六",};
            uint16_t x = 0, y = 14;
            sprintf(str, "%u", cdr->w_month);
            iot_paint_draw_string(x, y + 2, str, &Font8); x += Font8.Width * strlen(str); memset(str, 0, 10);
            iot_paint_draw_string(x, y, "月", &Font10_hz); x += Font10_hz.Width;

            sprintf(str, "%u", cdr->w_date);
            iot_paint_draw_string(x, y + 2, str, &Font8); x += Font8.Width * strlen(str); memset(str, 0, 10);
            iot_paint_draw_string(x, y, "日", &Font10_hz); x += Font10_hz.Width;

            x += 3;
            iot_paint_draw_string(x, y, "周", &Font10_hz); x += Font10_hz.Width;
            sprintf(str, "%u", cdr->week);
            iot_paint_draw_string(x, y, Cdr_i2week[cdr->week], &Font10_hz);
            {
                float temperature=0, humidity=0;
                sht3cx_get_data(&humidity, &temperature);
                ESP_LOGI(TAG, "temp=%2.1f, humi=%2.1f", temperature, humidity);
                sprintf(str, "%2.1fC %2.0f%%", temperature, humidity);

                // iot_paint_draw_string(0, 16, );
                iot_paint_draw_string(0, 25, str, &Font8);
                memset(str, 0, sizeof(str));
            }

            vTaskDelay(25000 / portTICK_PERIOD_MS);
        }
        
        {
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
            paint_roll_text_create(2, 16, 60);
            paint_roll_text_register_done_cb(toll_done_cb);
            paint_roll_text_set_string(str, &Font16_gbk);
            xEventGroupWaitBits(xCreatedEventGroup, BIT0, pdTRUE, pdFALSE, portMAX_DELAY);
            paint_roll_text_delete();
        }
        {
            memset(str, 0, sizeof(str));
            weather_get_str("shanghai", WEATHER_TYPE_NOW, str);

            paint_roll_text_create(2, 16, 60);
            paint_roll_text_register_done_cb(toll_done_cb);
            paint_roll_text_set_string(str, &Font16_gbk);
            xEventGroupWaitBits(xCreatedEventGroup, BIT0, pdTRUE, pdFALSE, portMAX_DELAY);
            paint_roll_text_delete();
        }
        {
            memset(str, 0, sizeof(str));
            weather_get_str("shanghai", WEATHER_TYPE_DAY, str);

            paint_roll_text_create(2, 16, 60);
            paint_roll_text_register_done_cb(toll_done_cb);
            paint_roll_text_set_string(str, &Font16_gbk);
            xEventGroupWaitBits(xCreatedEventGroup, BIT0, pdTRUE, pdFALSE, portMAX_DELAY);
            paint_roll_text_delete();
        }

    }
}

static void paint_handler_task(void *args)
{
    while (1) {
        vTaskDelay(30 / portTICK_PERIOD_MS);
        paint_handler();
    }
}
#define USE_LED_MATRIX CONFIG_USE_LEDMATRIX
void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    board_init();

    /** Determine whether to restore the settings by reading the restart count */
    int restart_cnt = restart_count_get();
    ESP_LOGI(TAG, "Restart count=[%d]", restart_cnt);
    if (restart_cnt >= RESTART_COUNT_RESET) {
        ESP_LOGW(TAG, "Erase information saved in flash and restart");
        ESP_ERROR_CHECK(nvs_flash_erase());
        esp_restart();
    }
    ESP_ERROR_CHECK(fm_init()); /* Initialize file storage */
#if USE_LED_MATRIX
    LedMatrix_init();
    lcd_driver_fun_t lcd_drv = {
        .draw_pixel = LedMatrix_DrawPoint,
        .draw_bitmap = LedMatrix_DrawBMP,
    };
#else
    lcd_sh1106_init();
    lcd_driver_fun_t lcd_drv = {
        .draw_pixel = lcd_sh1106_draw_4pixel,
        .draw_bitmap = lcd_sh1106_draw_bitmap,
    };
#endif
    
    iot_paint_init(&lcd_drv);
    LedMatrix_SetLight(4095);
    xTaskCreate(&paint_handler_task, "paint", 1024 * 2, NULL, 6, NULL);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    bool is_configured;
    captive_portal_start("Hello-clk", NULL, &is_configured);
    if (is_configured) {
        wifi_config_t wifi_config;
        esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config);
        char str[84];
        sprintf(str, "正在连接%s", wifi_config.sta.ssid);
        paint_roll_text_create(0, 16, 64);
        paint_roll_text_set_string(str, &Font16_gbk);
        ESP_LOGI(TAG, "SSID:%s, PASSWORD:%s", wifi_config.sta.ssid, wifi_config.sta.password);
        ret = captive_portal_wait(30000 / portTICK_PERIOD_MS);
        if (ESP_OK != ret) {
            ESP_LOGE(TAG, "Connect to AP timeout, restart to entry configuration");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            nvs_flash_erase();
            esp_restart();
        }
    } else {
        paint_roll_text_create(0, 16, 64);
        paint_roll_text_set_string("未联网，请进入192.168.4.1进行配网", &Font16_gbk);
        ret = captive_portal_wait(portMAX_DELAY);
    }

    sntp_start();
    paint_roll_text_delete();

    start_file_server();

    xTaskCreate(&Display_task, "Display_task", 1024 * 2, NULL, 6, NULL);
    xTaskCreate(&net_handle_task, "net_handle_task", 1024 * 6, NULL, 5, NULL);

    mp3_player_init();
    // mp3_player_start("/spiffs/01.mp3");
    mp3_player_set_volume(0);
    esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
    // app_bt_init();
}

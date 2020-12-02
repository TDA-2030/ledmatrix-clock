

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include <time.h>
#include <sys/time.h>
#include <string.h>

#include "shtcx.h"
#include "led_matrix.h"
#include "captive_portal.h"
#include "file_manage.h"
#include "file_server.h"
#include "weather.h"
#include "app_sntp.h"
#include "app_main.h"
#include "calendar.h"
#include "mp3_player.h"
#include "lcd_paint.h"

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
    iot_paint_Set_point_color(COLOR_GREEN);

    uint8_t times = 0, x = 4, y = 0;
    paint_roll_text_create(2, 16, 60);
    paint_roll_text_set_string("请你为我再将双手舞动，我会知道你在那个角落，看人生匆匆，愿我们同享光荣，愿我们的梦永不落空", &Font16_gbk);

    LedMatrix_Fill(x + 17, y + 4, x + 18, y + 5, iot_paint_Get_point_color());
    LedMatrix_Fill(x + 17, y + 10, x + 18, y + 11, iot_paint_Get_point_color());
    LedMatrix_Fill(x + 37, y + 4, x + 38, y + 5, iot_paint_Get_point_color());
    LedMatrix_Fill(x + 37, y + 10, x + 38, y + 11, iot_paint_Get_point_color());
    calendar_t *cdr = get_calendar_time();
    while (1) {
        if (++times > 33) {
            times = 0;
            cdr = get_calendar_time();
            // LedMatrix_SetLight(get_right_brightness(cdr->hour));
        }
        paint_show_clock(x, y, cdr->hour, cdr->min, cdr->sec);
        vTaskDelay(30 / portTICK_PERIOD_MS);
        paint_roll_text_handler();
    }
}

void net_handle_task(void *pvParameter)
{

    char str[128];

    sht3cx_init();


    while (1) {
        calendar_t *cdr = get_calendar_time();
        memset(str, 0, sizeof(str));
        calendar_get_lunar_str(cdr, str); printf("农历: %s\n", str);
        calendar_get_jieqi_str(cdr, str); printf("节气: %s\n", str);
        
        // weather_get("jian", WEATHER_TYPE_DAY);
        // weather_get("jian", WEATHER_TYPE_NOW);

        float temperature, humidity;
        sht3cx_get_data(&humidity, &temperature);
        ESP_LOGI(TAG, "h:%2.f, t:%.2f", humidity, temperature);

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

    ESP_ERROR_CHECK(fm_init()); /* Initialize file storage */

    LedMatrix_init();

    lcd_driver_fun_t lcd_drv = {
        .draw_pixel = LedMatrix_DrawPoint,
        .draw_bitmap = LedMatrix_DrawBMP,
    };
    iot_paint_init(&lcd_drv);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    LedMatrix_SetLight(100);
    iot_paint_draw_string(0, 0, "余海茗-", &Font16_gbk);
    
    iot_paint_draw_gbk_char_offset(18, 16, 0, 16, "与", &Font16_gbk);
    for (size_t i = 0; i < 12; i++)
    {
        iot_paint_draw_gbk_char_offset(0, 16, i, 4, "与", &Font16_gbk);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    

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
    
    start_file_server();

    xTaskCreate(&Display_task, "Display_task", 1024 * 2, NULL, 6, NULL);
    xTaskCreate(&net_handle_task, "net_handle_task", 1024 * 6, NULL, 5, NULL);

    mp3_player_init();
    mp3_player_start("/spiffs/01.mp3");
    mp3_player_set_volume(-8);
}

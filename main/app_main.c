

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "tcpip_adapter.h"
#include "nvs_flash.h"

#include "sht20.h"
#include "led_matrix.h"
#include "captive_portal.h"
#include "weather.h"
#include "app_sntp.h"
#include "app_main.h"

#include <time.h>
#include <sys/time.h>

static const char *TAG = "app_main";

struct tm timeinfo = {0};
SysStatus_t SystemStatus = 0;

void Display_task(void *pvParameter)
{
    LedMatrix_init();
    vTaskDelay(700 / portTICK_PERIOD_MS);
    LedMatrix_Clear();

    while (1)
    {
        // if (++times > 10)
        // {
        //     times = 0;

        //     even_bit = xEventGroupWaitBits(s_wifi_event_group, SNTP_TIME_READY_BIT, true, true, 0);
        //     if (even_bit & SNTP_TIME_READY_BIT)
        //     {
                
        //         printf("stm32 set time\n");
                
        //     }
        // }
        
        //if (time_last != 0)
        {
            uint8_t x = 3, y = 0;
            
            LedMatrix_ShowNum(x, y, timeinfo.tm_hour, 2, 16);
            LedMatrix_Fill(x + 17, y + 4, x + 18, y + 5, 1);
            LedMatrix_Fill(x + 17, y + 10, x + 18, y + 11, 1);
            LedMatrix_ShowNum(x + 20, y, timeinfo.tm_min, 2, 16);
            LedMatrix_Fill(x + 37, y + 4, x + 38, y + 5, 1);
            LedMatrix_Fill(x + 37, y + 10, x + 38, y + 11, 1);
            LedMatrix_ShowNum(x + 40, y, timeinfo.tm_sec, 2, 16);

            // LedMatrix_ShowNum(0, 16, sht20Info.humidity * 100, 4, 12);
            // LedMatrix_ShowNum(33, 16, sht20Info.tempreture * 100, 4, 12);
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void net_handle_task(void *pvParameter)
{
    app_sntp_initialize();
    
    while (1)
    {
        app_sntp_get_time(&timeinfo);
       
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        // weather_get("jian", WEATHER_TYPE_DAY);
        weather_get("jian", WEATHER_TYPE_NOW);
    }
}

void misc_task(void *pvParameter)
{

    vTaskDelay(100 / portTICK_PERIOD_MS);
    while (1)
    {
       ESP_LOGI(TAG, "misc");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static char *String lunarMonthName[] = {
            "腊", "正", "二", "三",
            "四", "五", "六", "七",
            "八", "九", "十", "冬"
    };
static char *String lunarDayName[] = {
        "三十", "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九",
        "初十", "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九",
        "二十", "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九"
};
//天干
static char *String nature[] = {
        "癸", "甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬"
};

//地支
static char *String earth[] = {
        "亥", "子", "丑", "寅", "卯", "辰", "巳", "午", "末", "申", "酉", "戌"
};

//属相名
static char *String animal[] = {
        "猪", "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗"
};

//节气
static char *String solarTerm[][] = {
        {"大雪", "冬至"}, {"小寒", "大寒"}, {"立春", "雨水"},
        {"惊蛰", "春分"}, {"清明", "谷雨"}, {"立夏", "小满"},
        {"芒种", "夏至"}, {"小暑", "大暑"}, {"立秋", "处暑"},
        {"白露", "秋分"}, {"寒露", "霜降"}, {"立冬", "小雪"}
};

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    bool is_configured;
    captive_portal_start("ESP_WEB_CONFIG", NULL, &is_configured);

    if (is_configured) {
        wifi_config_t wifi_config;
        esp_wifi_get_config(ESP_IF_WIFI_STA, &wifi_config);
        ESP_LOGI(TAG, "SSID:%s, PASSWORD:%s", wifi_config.sta.ssid, wifi_config.sta.password);
        ret = captive_portal_wait(30000 / portTICK_PERIOD_MS);
        if(ESP_OK != ret){
            ESP_LOGE(TAG, "Connect to AP timeout, restart to entry configuration");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            nvs_flash_erase();
            esp_restart();
        }
    } else {
        ret = captive_portal_wait(600*1000 / portTICK_PERIOD_MS);
    }
    
    xTaskCreate(&misc_task, "misc_task", 1024 * 2, NULL, 5, NULL);
    xTaskCreate(&Display_task, "Display_task", 1024 * 2, NULL, 6, NULL);
    xTaskCreate(&net_handle_task, "net_handle_task", 1024 * 6, NULL, 5, NULL);
}

/* SPI Master example


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "tcpip_adapter.h"
#include "esp_smartconfig.h"
#include "nvs_flash.h"

#include "sht20.h"
#include "rtc_time.h"
#include "led_matrix.h"
#include "nvs_storage.h"
#include "net_config.h"
#include "weather.h"
#include "app_sntp.h"
#include "app_main.h"

#include <time.h>
#include <sys/time.h>

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

    xEventGroupWaitBits(s_wifi_event_group, APPWIFI_CONNECTED_BIT, false, true, portMAX_DELAY);


    app_sntp_initialize();

    
    while (1)
    {

        app_sntp_get_time(&timeinfo);
       
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // weather_get("jian", WEATHER_TYPE_DAY);
        // weather_get("jian", WEATHER_TYPE_NOW);
    }
}

void misc_task(void *pvParameter)
{
    uint8_t times = 0;
    pvParameter = pvParameter;

    vTaskDelay(100 / portTICK_PERIOD_MS);
    while (1)
    {
       
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    app_wifi_init();
    printf("\nLED Matrix Clock\n\n");

    xTaskCreate(&misc_task, "misc_task", 1024 * 2, NULL, 5, NULL);
    xTaskCreate(&Display_task, "Display_task", 1024 * 2, NULL, 6, NULL);
    xTaskCreate(&net_handle_task, "net_handle_task", 1024 * 6, NULL, 5, NULL);
}

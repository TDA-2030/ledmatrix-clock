/* ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "tcpip_adapter.h"
#include "esp_smartconfig.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "net_config.h"
#include "app_main.h"

/* The examples use WiFi configuration that you can set via 'make menuconfig'.

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/


static const char *TAG = "app wifi";

EventGroupHandle_t s_wifi_event_group;
const int APPWIFI_CONNECTED_BIT     = BIT0;
const int APPWIFI_ESPTOUCH_DONE_BIT = BIT1;


static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "WiFi Disonnected to ap");
        SYS_STATUS_CLR(SYS_STATUS_CONNECTED);
        xEventGroupClearBits(s_wifi_event_group, APPWIFI_CONNECTED_BIT);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI(TAG, "WiFi Connected to ap");
        SYS_STATUS_SET(SYS_STATUS_CONNECTED);
        xEventGroupSetBits(s_wifi_event_group, APPWIFI_CONNECTED_BIT);
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "Scan done");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL)
    {
        ESP_LOGI(TAG, "Found channel");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true)
        {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }
        ESP_LOGI(TAG, "SSID:%s", wifi_config.sta.ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", wifi_config.sta.password);

        //ESP_ERROR_CHECK( esp_wifi_disconnect() );
        esp_err_t err = ( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
        ESP_LOGI(TAG, "esp_wifi_set_config result:%s", esp_err_to_name(err));
        err = ( esp_wifi_connect() );
        ESP_LOGI(TAG, "esp_wifi_connect result:%s", esp_err_to_name(err));
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE)
    {
        xEventGroupSetBits(s_wifi_event_group, APPWIFI_ESPTOUCH_DONE_BIT);
        ESP_LOGI(TAG, "smartconfig over");
    }
}


static void app_wifi_task(void *parm)
{
    EventBits_t uxBits;
    uint16_t time = 15;
    while (time--) /* 等待WiFi自动连接 */
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        uxBits = xEventGroupGetBits(s_wifi_event_group);
        if (uxBits & APPWIFI_CONNECTED_BIT)
        {
            break;
        }
    }
    /* 没连接上则开启smartconfig */
    if (!(uxBits & APPWIFI_CONNECTED_BIT))
    {
        ESP_LOGI(TAG, "WiFi smartconfig starting");
        ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH_AIRKISS) );
        smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
        ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
        SYS_STATUS_SET(SYS_STATUS_SMARTCONFIG);
    }
    while (1)
    {
        uxBits = xEventGroupGetBits(s_wifi_event_group);
        if (uxBits & APPWIFI_CONNECTED_BIT)
        {
            
        }
        if (uxBits & APPWIFI_ESPTOUCH_DONE_BIT)
        {
            xEventGroupClearBits(s_wifi_event_group, APPWIFI_ESPTOUCH_DONE_BIT);
            esp_smartconfig_stop();
            SYS_STATUS_CLR(SYS_STATUS_SMARTCONFIG);
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}



void app_wifi_init(void)
{
    
    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    // wifi_config_t wifi_config = {
    //     .sta = {
    //         .ssid = CONFIG_ESP_WIFI_SSID,
    //         .password = CONFIG_ESP_WIFI_PASSWORD,
    //     },
    // };
    // ESP_LOGI(TAG, "Connecting to %s...", wifi_config.sta.ssid);
    // ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    xTaskCreate(app_wifi_task, "app_wifi_task", 4096, NULL, 3, NULL);
}

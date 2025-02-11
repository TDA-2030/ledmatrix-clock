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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "adapt/esp32_wifi.h"
#include "adapt/esp32_httpd.h"
#include "cgi/cgiwifi.h"
#include "captive_portal.h"
#include "dns_server.h"

static const char *TAG = "captive_portal";
static bool g_configed = 0;
static esp_timer_handle_t prov_stop_timer;
dns_server_handle_t dns_server;

esp_err_t config_timer_start(int TIMEOUT_PERIOD);
esp_err_t config_timer_delete(void);

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == APP_NETWORK_EVENT) {
        switch (event_id) {
        case APP_NETWORK_EVENT_CONFIG_SUCCESS:
        case APP_NETWORK_EVENT_PROV_TIMEOUT:
            config_timer_delete();
            stop_dns_server(dns_server);
            esp32HttpServerDisable();
            break;
        case APP_NETWORK_EVENT_PROV_START:
            break;

        default:
            ESP_LOGW(TAG, "Unhandled App Wi-Fi Event: %"PRIi32, event_id);
            break;
        }
    }
}

esp_err_t captive_portal_start(void)
{
    esp_err_t ret;

    // Start the DNS server that will redirect all queries to the softAP IP
    dns_server_config_t config = DNS_SERVER_CONFIG_SINGLE("*" /* all A queries */, "WIFI_AP_DEF" /* softAP netif ID */);
    dns_server = start_dns_server(&config);

    /* start http server task */
    ESP_LOGD(TAG, "Free heap size before enable http server: %d", esp_get_free_heap_size());
    ret = esp32HttpServerEnable();
    if (ESP_OK != ret) {
        return ESP_FAIL;
    }

    ESP_ERROR_CHECK(esp_event_handler_register(APP_NETWORK_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    config_timer_start(30);
    esp_event_post(APP_NETWORK_EVENT, APP_NETWORK_EVENT_PROV_START, NULL, 0, portMAX_DELAY);
    ESP_LOGI(TAG, "Http server ready ...");

    return ESP_OK;
}


static void wifi_config_stop(void *priv)
{
    ESP_LOGW(TAG, "Provisioning timed out. Please reboot device to restart provisioning.");
    esp_event_post(APP_NETWORK_EVENT, APP_NETWORK_EVENT_PROV_TIMEOUT, NULL, 0, portMAX_DELAY);
}

esp_err_t config_timer_start(int TIMEOUT_PERIOD)
{
    if (TIMEOUT_PERIOD == 0) {
        return ESP_OK;
    }
    uint64_t prov_timeout_period = (TIMEOUT_PERIOD * 60 * 1000000LL);

    esp_timer_create_args_t prov_stop_timer_conf = {
        .callback = wifi_config_stop,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "app_wifi_config_stop_tm"
    };
    if (esp_timer_create(&prov_stop_timer_conf, &prov_stop_timer) == ESP_OK) {
        esp_timer_start_once(prov_stop_timer, prov_timeout_period);
        ESP_LOGI(TAG, "Provisioning will auto stop after %d minute(s).", TIMEOUT_PERIOD);
        return ESP_OK;
    } else {
        ESP_LOGE(TAG, "Failed to create Provisioning auto stop timer.");
    }
    return ESP_FAIL;
}

esp_err_t config_timer_delete(void)
{
    if (prov_stop_timer) {
        esp_timer_stop(prov_stop_timer);
        esp_timer_delete(prov_stop_timer);
        prov_stop_timer = NULL;
    }
    return ESP_OK;
}

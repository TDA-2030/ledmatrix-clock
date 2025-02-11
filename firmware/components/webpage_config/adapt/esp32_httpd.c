#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "esp_log.h"
#include "esp_err.h"
#include "../cgi/cgiwifi.h"


static const char *TAG = "esp32_httpd";

static httpd_handle_t g_server;

static const httpd_uri_t builtInUrls[] = {
    {.uri = "/wifiscan.cgi",       .method = HTTP_GET,    .handler = cgiWiFiScan,            .user_ctx  = NULL},
    {.uri = "/connect.cgi",        .method = HTTP_POST,   .handler = cgiWiFiConnect,         .user_ctx  = NULL},
    {.uri = "/connstatus.cgi",     .method = HTTP_GET,    .handler = cgiWiFiConnStatus,      .user_ctx  = NULL},
    {.uri = "/configsuccess.cgi",  .method = HTTP_POST,   .handler = cgiWiFiConfigSuccess,   .user_ctx  = NULL},
    {.uri = "/*",                  .method = HTTP_GET,    .handler = cgi_common_get_handler, .user_ctx  = NULL},
    // {.uri = "/setmode.cgi",        .method = HTTP_GET,    .handler = cgiWiFiSetMode,         .user_ctx  = NULL},
};

// Enable http server
esp_err_t esp32HttpServerEnable(void)
{

#ifndef CONFIG_WEB_SSL_SUPPORT
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    int handlers_num = sizeof(builtInUrls) / sizeof(httpd_uri_t);

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);

    if (httpd_start(&g_server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");

        for (int i = 0; i < handlers_num; i++) {
            if (httpd_register_uri_handler(g_server, &builtInUrls[i]) != ESP_OK) {
                ESP_LOGE(TAG, "register uri failed for %d", i);
                httpd_stop(g_server);
                return ESP_FAIL;
            }
        }
    }

#else
//     int listenPort = 443;
//     httpdFreertosInit(&httpdFreertosInstance, builtInUrls, listenPort,
//                       connectionMemory, HTTPD_MAX_CONNECTIONS, HTTPD_FLAG_SSL);
#endif

    return ESP_OK;
}

// Disable http server
esp_err_t esp32HttpServerDisable(void)
{
    httpd_stop(g_server);
    return ESP_OK;
}


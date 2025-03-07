#ifndef __ESP32_WIFI_H__
#define __ESP32_WIFI_H__

#include "esp_wifi_types.h"
#include "freertos/event_groups.h"

// --------------------------------------------------------------------------
// external variables
// --------------------------------------------------------------------------
extern EventGroupHandle_t g_wifi_event_group;
extern const int WIFI_SCAN_DONE;
extern const int WIFI_STA_GOT_IP;
extern const int WIFI_STA_CONNECTED;
extern const int WIFI_STA_DISCONNECT;
ESP_EVENT_DECLARE_BASE(APP_NETWORK_EVENT);

/** App Network Events */
typedef enum {
    APP_NETWORK_EVENT_PROV_START = 1,
    APP_NETWORK_EVENT_CONFIG_SUCCESS,
    APP_NETWORK_EVENT_PROV_TIMEOUT,
} app_network_event_t;


bool wifiConnect(void);
void wifiDisconnect(void);
void wifiStartScan(void);
void wifiStopScan(void);
uint16_t wifiScanDone(wifi_ap_record_t **ap_records);

esp_err_t wifiIinitialize(const char *ap_ssid, const char *ap_pwd, bool *configured);
esp_err_t wifiConnected(uint32_t ticks_to_wait);

void wifiShowStatus(void);
void wifiStartWps(void);

void wifiSetNewMode(wifi_mode_t mode);
bool wifiGetCurrentMode(wifi_mode_t *mode);
esp_err_t wifiSetConfig(wifi_interface_t interface, wifi_config_t *conf);
esp_err_t wifiGetConfig(wifi_interface_t interface, wifi_config_t *conf);
esp_err_t wifiSetChannel(uint8_t primary, wifi_second_chan_t second);

#endif // __ESP32_WIFI_H__
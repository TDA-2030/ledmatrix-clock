#ifndef __NET_CONFIG__H
#define __NET_CONFIG__H


#ifdef __cplusplus
extern "C" {
#endif



extern EventGroupHandle_t s_wifi_event_group;

extern const int APPWIFI_CONNECTED_BIT;


void app_wifi_init();

#ifdef __cplusplus
}
#endif

#endif
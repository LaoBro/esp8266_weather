#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <FreeRTOS.h>
#include <event_groups.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include "esp_smartconfig.h"
#include "smartconfig_ack.h"

/* FreeRTOS event group to signal when we are connected & ready to make a request */
extern EventGroupHandle_t s_wifi_event_group;
/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
extern const int CONNECTED_BIT;
extern const int ESPTOUCH_DONE_BIT;

void wifi_init();

#endif /* MY_WIFI_H */

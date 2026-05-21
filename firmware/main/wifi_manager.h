#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0

void wifi_init(EventGroupHandle_t event_group);

#endif
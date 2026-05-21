#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "freertos/event_groups.h"
#include "mqtt_client.h"

#define MQTT_CONNECTED_BIT BIT1

void mqtt_start(EventGroupHandle_t event_group);
esp_mqtt_client_handle_t mqtt_get_client(void);

#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_timer.h"
#include "mqtt_client.h"

#include "nvs_flash.h"
#include "nvs.h"

#include "telemetry.h"
#include "mqtt_manager.h"

#define TELEMETRY_TOPIC "edgepulse/manjunatha144/device_001/telemetry"

#define WIFI_CONNECTED_BIT BIT0
#define MQTT_CONNECTED_BIT BIT1

#define MIN_INTERVAL_MS 500
#define MAX_INTERVAL_MS 60000

static const char *TAG = "TELEMETRY";

static EventGroupHandle_t system_event_group;

/* Persistent interval */
static volatile uint32_t interval_ms = 5000;

/* Counters */
static uint32_t sent_count = 0;
static uint32_t failed_count = 0;
static uint32_t reconnect_count = 0;


/* ---------- Load Interval From NVS ---------- */
static void load_interval_from_nvs()
{
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        uint32_t saved_interval;
        err = nvs_get_u32(nvs_handle, "interval", &saved_interval);
        if (err == ESP_OK) {
            interval_ms = saved_interval;
            ESP_LOGI(TAG, "Loaded interval from NVS: %lu", interval_ms);
        }
        nvs_close(nvs_handle);
    }
}

/* ---------- Save Interval To NVS ---------- */
static void save_interval_to_nvs(uint32_t value)
{
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err == ESP_OK) {
        nvs_set_u32(nvs_handle, "interval", value);
        nvs_commit(nvs_handle);
        nvs_close(nvs_handle);
        ESP_LOGI(TAG, "Saved interval to NVS: %lu", value);
    }
}

/* ---------- Public Setter ---------- */
void telemetry_set_interval(uint32_t value)
{
    if (value >= MIN_INTERVAL_MS && value <= MAX_INTERVAL_MS) {
        interval_ms = value;
        save_interval_to_nvs(value);
        ESP_LOGI(TAG, "Interval updated to %lu ms", interval_ms);
    } else {
        ESP_LOGW(TAG, "Invalid interval received: %lu", value);
    }
}

/* ---------- Reconnect Notifier ---------- */
void telemetry_notify_reconnect(void)
{
    reconnect_count++;
    ESP_LOGI(TAG, "Reconnect count incremented: %lu", reconnect_count);
}

/* ---------- Telemetry Task ---------- */
static void telemetry_task(void *arg)
{
    uint32_t seq = 0;

    while (1) {

        xEventGroupWaitBits(system_event_group,
                            WIFI_CONNECTED_BIT | MQTT_CONNECTED_BIT,
                            pdFALSE,
                            pdTRUE,
                            portMAX_DELAY);

        seq++;

        uint32_t uptime_ms = esp_timer_get_time() / 1000;

        char payload[256];
        snprintf(payload, sizeof(payload),
                 "{\"device_id\":\"device_001\","
                 "\"seq\":%lu,"
                 "\"uptime_ms\":%lu,"
                 "\"temp\":27.5,"
                 "\"sent\":%lu,"
                 "\"failed\":%lu,"
                 "\"reconnect\":%lu}",
                 seq, uptime_ms, sent_count, failed_count, reconnect_count);

        int msg_id = esp_mqtt_client_publish(mqtt_get_client(),
                                             TELEMETRY_TOPIC,
                                             payload,
                                             0, 0, 0);

        if (msg_id >= 0) {
            sent_count++;
        } else {
            failed_count++;
        }

        ESP_LOGI(TAG, "Telemetry sent: %s", payload);

        vTaskDelay(pdMS_TO_TICKS(interval_ms));
    }
}

/* ---------- Start Telemetry ---------- */
void telemetry_start(EventGroupHandle_t event_group)
{
    system_event_group = event_group;

    load_interval_from_nvs();

    xTaskCreate(telemetry_task,
                "telemetry_task",
                4096,
                NULL,
                5,
                NULL);
}
#ifndef __WEATHER_H__
#define __WEATHER_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "string.h"
#include "esp_log.h"
#include "esp_http_client.h"  // 替换为ESP-IDF自带的HTTP客户端
#include "cJSON.h"

extern const unsigned char server_root_cert_pem_start[]   asm("_binary_seniverse_com_chain_pem_start");
extern const unsigned char server_root_cert_pem_end[]     asm("_binary_seniverse_com_chain_pem_end");

// 定义天气数据结构体
typedef struct {
    char weather[16];
    char temp[16];
    char humidity[16];
} weather_data_t;

extern weather_data_t latest_weather_data;

void weather_init(void);
void weather_update_task(void *pvParameters);
void parse_weather_data(const char *json_str);

#endif
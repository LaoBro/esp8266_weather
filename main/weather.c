#include "weather.h"
#include "esp_tls.h"

#define WEATHER_API_URL "https://api.seniverse.com/v3/weather/now.json"
#define API_KEY "SRRAJPAeU9ciC8Vsk" // 替换为你的心知天气API密钥
#define LOCATION "Foshan"           // 替换为你需要的城市拼音
#define LANGUAGE "en"               // 新增语言选项宏定义
#define MAX_HTTP_RECV_BUFFER 2048

weather_data_t latest_weather_data = {0};

void fetch_weather()
{
    char request[512];
    snprintf(request, sizeof(request),
             "GET /v3/weather/now.json?key=%s&location=%s&language=%s&unit=c HTTP/1.1\r\n"
             "Host: api.seniverse.com\r\n"
             "User-Agent: ESP8266\r\n"
             "Connection: close\r\n\r\n",
             API_KEY, LOCATION, LANGUAGE);

    esp_tls_cfg_t cfg = {
        .cacert_buf = (const unsigned char *)server_root_cert_pem_start,
        .cacert_bytes = server_root_cert_pem_end - server_root_cert_pem_start,
    };

    esp_tls_t *tls = esp_tls_conn_http_new("https://api.seniverse.com", &cfg);
    if (!tls)
    {
        ESP_LOGE("WEATHER", "Connection failed");
        return;
    }

    int ret = esp_tls_conn_write(tls, request, strlen(request));
    if (ret < 0)
    {
        ESP_LOGE("WEATHER", "Write error");
        esp_tls_conn_delete(tls);
        return;
    }

    char buffer[MAX_HTTP_RECV_BUFFER] = {0};
    int total_len = 0;

    while (total_len < MAX_HTTP_RECV_BUFFER - 1)
    {
        ret = esp_tls_conn_read(tls, buffer + total_len, MAX_HTTP_RECV_BUFFER - total_len - 1);
        if (ret == MBEDTLS_ERR_SSL_WANT_READ)
            continue;
        if (ret < 0)
            break;
        if (ret == 0)
            break;
        total_len += ret;
    }

    buffer[total_len] = '\0';
    ESP_LOGI("WEATHER", "Response: %s", buffer);

    char *json_start = strstr(buffer, "\r\n\r\n");
    if (json_start)
    {
        // 解析 JSON 数据
        json_start += 4; // 跳过空行
        parse_weather_data(json_start);
    } else {
        ESP_LOGE("WEATHER", "No JSON data found");
    }

    esp_tls_conn_delete(tls);
}

void weather_init(void)
{

    // 创建更新任务
    xTaskCreate(weather_update_task, "weather_update", 8192, NULL, 3, NULL);
}

void weather_update_task(void *pvParameters)
{

    while (1)
    {
        UBaseType_t watermark = uxTaskGetStackHighWaterMark(NULL);
        ESP_LOGI("STACK", "Free stack: %u", watermark);
        fetch_weather();

        // 每10分钟秒更新一次
        vTaskDelay(pdMS_TO_TICKS(10 * 60 * 1000));
    }
}

void parse_weather_data(const char *json_str)
{
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL)
    {
        ESP_LOGE("WEATHER", "Failed to parse JSON");
        return;
    }

    cJSON *results = cJSON_GetObjectItem(root, "results");
    if (results == NULL)
    {
        ESP_LOGE("WEATHER", "No results in JSON");
        cJSON_Delete(root);
        return;
    }

    cJSON *first_result = cJSON_GetArrayItem(results, 0);
    if (first_result == NULL)
    {
        ESP_LOGE("WEATHER", "No data in results");
        cJSON_Delete(root);
        return;
    }

    cJSON *now = cJSON_GetObjectItem(first_result, "now");
    if (now == NULL)
    {
        ESP_LOGE("WEATHER", "No now data");
        cJSON_Delete(root);
        return;
    }

    cJSON *location = cJSON_GetObjectItem(first_result, "location");
    if (location == NULL)
    {
        ESP_LOGE("WEATHER", "No location data");
        cJSON_Delete(root);
        return;
    }

    // 解析天气数据
    cJSON *weather = cJSON_GetObjectItem(now, "text");
    cJSON *temp = cJSON_GetObjectItem(now, "temperature");
    cJSON *humidity = cJSON_GetObjectItem(now, "humidity");

    // 填充到全局结构体
    if (weather)
        strncpy(latest_weather_data.weather, weather->valuestring, sizeof(latest_weather_data.weather));
    if (temp)
        strncpy(latest_weather_data.temp, temp->valuestring, sizeof(latest_weather_data.temp));
    if (humidity)
        strncpy(latest_weather_data.humidity, humidity->valuestring, sizeof(latest_weather_data.humidity));

    cJSON_Delete(root);
}
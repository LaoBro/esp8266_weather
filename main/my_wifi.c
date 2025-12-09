#include "my_wifi.h"

/* The examples use smartconfig type that you can set via project configuration menu.

   If you'd rather not, just change the below entries to enum with
   the config you want - ie #define EXAMPLE_ESP_SMARTCOFNIG_TYPE SC_TYPE_ESPTOUCH
*/
#define EXAMPLE_ESP_SMARTCOFNIG_TYPE      SC_TYPE_ESPTOUCH

/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;
const int ESPTOUCH_DONE_BIT = BIT1;

// 添加Wi-Fi事件处理函数
// static esp_err_t wifi_event_handler(void *ctx, system_event_t *event) {
//     if (event->event_id == SYSTEM_EVENT_STA_START) {
//         esp_wifi_connect();
//     } else if (event->event_id == SYSTEM_EVENT_STA_DISCONNECTED) {
//         esp_wifi_connect();
//     }
//     return ESP_OK;
// }

static const char *TAG = "my_wifi";
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

static void ip_event_handler(void *arg, esp_event_base_t event_base,
                             int32_t event_id, void *event_data)
{
    if (event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI(TAG, "ESP_GOT_IP");
        xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
    }
}

void wifi_init()
{
    // 初始化 NVS
    ESP_ERROR_CHECK(nvs_flash_init());

    // 初始化事件循环 (ESP-IDF 3.x版本)
    // esp_event_loop_init(wifi_event_handler, NULL);
    tcpip_adapter_init();

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, NULL));

    // 初始化 Wi-Fi
    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);

    // 配置 Wi-Fi 连接信息
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = ""}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
    //esp_wifi_connect();
}

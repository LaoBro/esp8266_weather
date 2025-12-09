/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_event.h"
#include "driver/uart.h"
#include "my_ssd1306.h"
#include "weather.h"
#include "my_wifi.h"
#include "my_sntp.h"
#include "my_ui.h"
#include "my_smartconfig.h"


void app_main()
{
    uart_set_baudrate(UART_NUM_0, 74880);

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    my_ssd1306_init();
    draw_smartconfig_ui();
    wifi_init();
    smartconfig_init();
    xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
    weather_init();
    my_sntp_init();
    ui_init();
    
    // 主循环不再需要处理天气显示
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("%s: system running\n", __FUNCTION__);
    }
    
}

#include "my_ui.h"

void ui_init()
{
    xTaskCreate(ui_task, "ui_task", 4096, NULL, 3, NULL);
}
void draw_ui(void)
{
    clear_frame_buffer();
    time_t now = 0;
    struct tm timeinfo = {0};
    time(&now);
    localtime_r(&now, &timeinfo);

    char strftime_buf[32];
    strftime(strftime_buf, sizeof(strftime_buf), "%H:%M:%S", &timeinfo);
    ssd1306_draw_string(&my_ssd1306, frame_buffer, my_font, 0, 0, strftime_buf, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%s\xb0", latest_weather_data.temp);
    ssd1306_draw_string(&my_ssd1306, frame_buffer, my_font, 0, 32, temp_str, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    ssd1306_draw_string(&my_ssd1306, frame_buffer, font_builtin_fonts[FONT_FACE_TERMINUS_8X14_ISO8859_1], 48, 41, latest_weather_data.weather, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    ssd1306_load_frame_buffer(&my_ssd1306, frame_buffer);
}

void draw_smartconfig_ui(void)
{
    clear_frame_buffer();
    ssd1306_draw_string(&my_ssd1306, frame_buffer, font_builtin_fonts[FONT_FACE_TERMINUS_8X14_ISO8859_1], 0, 0, "Waiting for:", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    ssd1306_draw_string(&my_ssd1306, frame_buffer, font_builtin_fonts[FONT_FACE_TERMINUS_8X14_ISO8859_1], 0, 15, "Esptouch or", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    ssd1306_draw_string(&my_ssd1306, frame_buffer, font_builtin_fonts[FONT_FACE_TERMINUS_8X14_ISO8859_1], 0, 30, "Airkiss", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    ssd1306_load_frame_buffer(&my_ssd1306, frame_buffer);
}

void ui_task(void *pvParameters)
{
    // 设置时区
    setenv("TZ", "CST-8", 1);
    tzset();
    while (1)
    {
        draw_ui();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
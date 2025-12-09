#ifndef MY_UI_H
#define MY_UI_H

#include <ssd1306/ssd1306.h>
#include <time.h>
#include "my_ssd1306.h"
#include "weather.h"

// 函数声明
void ui_init(void);
void draw_ui(void);
void draw_smartconfig_ui(void);
void ui_task(void *pvParameters);

#endif // MY_UI_H
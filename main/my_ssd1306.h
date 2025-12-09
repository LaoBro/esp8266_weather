#ifndef MY_SSD1306_H
#define MY_SSD1306_H

#include "ssd1306/ssd1306.h"
#include "driver/i2c.h"
#include "freertos/task.h"
#include "string.h"
#include "image.xbm"

#define SCL_PIN 2
#define SDA_PIN 0
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define my_i2c_port I2C_NUM_0

// 修改为extern声明
extern const font_info_t *my_font;
extern uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];
extern ssd1306_t my_ssd1306;
extern i2c_config_t my_i2c_config;

void my_ssd1306_init(void);
void clear_frame_buffer(void);
void my_ssd1306_test(void);

#endif // MY_SSD1306_H
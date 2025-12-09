#include "my_ssd1306.h"

const font_info_t *my_font;
i2c_config_t my_i2c_config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = SDA_PIN,
    .sda_pullup_en = 1,
    .scl_io_num = SCL_PIN,
    .scl_pullup_en = 1,
    .clk_stretch_tick = 300};
uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];
ssd1306_t my_ssd1306 = {
    .i2c_port = my_i2c_port,
    .i2c_addr = SSD1306_I2C_ADDR_0,
    .screen = SSD1306_SCREEN,
    .width = DISPLAY_WIDTH,
    .height = DISPLAY_HEIGHT};

void my_ssd1306_init(void)
{
    my_font = font_builtin_fonts[FONT_FACE_TERMINUS_16X32_ISO8859_1];
    // init i2c
    printf("Init I2C\n");

    ESP_ERROR_CHECK(i2c_driver_install(my_i2c_port, my_i2c_config.mode));
    ESP_ERROR_CHECK(i2c_param_config(my_i2c_port, &my_i2c_config));

    // init ssd1306
    printf("Init SSD1306\n");

    uint8_t pin_cfg;
    switch (my_ssd1306.height)
    {
    case 16:
        pin_cfg = 0x02;
        break;
    case 32:
        if (my_ssd1306.width == 128)
            pin_cfg = 0x02;
        else
            pin_cfg = 0x12;
        break;
    case 64:
        pin_cfg = 0x12;
        break;
    default:
        printf("Unsupported screen height");
        while (true)
        {
        };
    }

    ssd1306_display_on(&my_ssd1306, false);
    ESP_ERROR_CHECK(ssd1306_set_osc_freq(&my_ssd1306, 0x80));
    ESP_ERROR_CHECK(ssd1306_set_mux_ratio(&my_ssd1306, my_ssd1306.height - 1));
    ESP_ERROR_CHECK(ssd1306_set_display_offset(&my_ssd1306, 0x0));
    ESP_ERROR_CHECK(ssd1306_set_display_start_line(&my_ssd1306, 0x0));
    ESP_ERROR_CHECK(ssd1306_set_charge_pump_enabled(&my_ssd1306, true));
    ESP_ERROR_CHECK(ssd1306_set_mem_addr_mode(&my_ssd1306, SSD1306_ADDR_MODE_HORIZONTAL));
    ESP_ERROR_CHECK(ssd1306_set_segment_remapping_enabled(&my_ssd1306, false));
    ESP_ERROR_CHECK(ssd1306_set_scan_direction_fwd(&my_ssd1306, true));
    ESP_ERROR_CHECK(ssd1306_set_com_pin_hw_config(&my_ssd1306, pin_cfg));
    ESP_ERROR_CHECK(ssd1306_set_contrast(&my_ssd1306, 0x9f));
    ESP_ERROR_CHECK(ssd1306_set_precharge_period(&my_ssd1306, 0xf1));
    ESP_ERROR_CHECK(ssd1306_set_deseltct_lvl(&my_ssd1306, 0x40));
    // ESP_ERROR_CHECK(ssd1306_set_whole_display_lighting(&my_ssd1306, true));
    ESP_ERROR_CHECK(ssd1306_set_inversion(&my_ssd1306, false));
    ESP_ERROR_CHECK(ssd1306_display_on(&my_ssd1306, true));
    // ESP_ERROR_CHECK(ssd1306_set_whole_display_lighting(&my_ssd1306, false));
    ESP_ERROR_CHECK(ssd1306_clear_screen(&my_ssd1306));
    ESP_ERROR_CHECK(ssd1306_stop_scroll(&my_ssd1306));
}

void clear_frame_buffer(void)
{
    memset(frame_buffer, 0, sizeof(frame_buffer));
}

void my_ssd1306_test(void)
{
    static bool fwd = false;
    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("%s: still alive, flipping!\n", __FUNCTION__);
    ssd1306_set_scan_direction_fwd(&my_ssd1306, fwd);
    fwd = !fwd;
    vTaskDelay(pdMS_TO_TICKS(1000));
    ssd1306_clear_screen(&my_ssd1306);
    clear_frame_buffer();
    vTaskDelay(pdMS_TO_TICKS(1000));
    ssd1306_draw_string(&my_ssd1306, frame_buffer, my_font, 0, 0, "Hello World!", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    ssd1306_draw_string(&my_ssd1306, frame_buffer, my_font, 0, 7, "Hello World!", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
    ssd1306_load_frame_buffer(&my_ssd1306, frame_buffer);
    vTaskDelay(pdMS_TO_TICKS(1000));
    ssd1306_clear_screen(&my_ssd1306);
    clear_frame_buffer();
    ssd1306_load_xbm(&my_ssd1306, image_bits, frame_buffer);
}
#pragma once

#include <U8g2lib.h>

#define SCREEN_ROTATION U8G2_R2
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(SCREEN_ROTATION, /*reset*/ OLED_RESET_PIN, /*clk*/ OLED_CLK_PIN, /*data*/ OLED_DTA_PIN);

void display_init() {

}

void display_clear() {
    display.clear();
    display.clearBuffer();
}

void display_flush() {
    display.sendBuffer();
}
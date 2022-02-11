#pragma once

#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

void display_init() {
    Wire.begin(OLED_DTA_PIN, OLED_CLK_PIN);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.display();
}

void display_clear() {
    display.clearDisplay();
}

void display_flush() {
    display.display();
}

void display_print(int x, int y, const char *text)
{
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.print(text);
}

void display_print_size(int x, int y, int size, const char *text)
{
  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.print(text);
}
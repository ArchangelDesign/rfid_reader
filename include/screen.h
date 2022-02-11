#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET_PIN 4
#define SCREEN_ADDRESS 0x3C

#ifndef OLED_RESET_PIN
#define OLED_RESET_PIN 16
#endif
#ifndef OLED_CLK_PIN
#define OLED_CLK_PIN 15
#endif
#ifndef OLED_DTA_PIN
#define OLED_DTA_PIN 4
#endif

bool refresh_screen();
void init_display();
void display_flush();
void display_print(int x, int y, const char *text);
void display_print_size(int x, int y, int size, const char *text);
void display_clear();
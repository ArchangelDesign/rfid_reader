#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

bool refresh_screen();
void init_display();
void display_flush();
void display_print(int x, int y, const char *text);
void display_print_size(int x, int y, int size, const char *text);
void display_clear();
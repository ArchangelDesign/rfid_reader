#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#include "flash.h"
#include "status.h"

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

const char f_frigger[] PROGMEM = "FRIGGER";
const char f_fridge_door_open[] PROGMEM = "fridge door open";
const char f_freezer_door_open[] PROGMEM = "freezer door open";
const char f_both_door_open[] PROGMEM = "both doors open";
const char f_done[] PROGMEM = "DONE";
const unsigned char PROGMEM bmp_dot[] = {
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b01111110,
    0b00111100,
    0b00011000
};

#if OLED_DRIVER_U8
#include "u8display.h"
#else
#include "adafruitdisplay.h"
#endif

void init_display() {
  #if OLED_DRIVER_U8
  log_d("Using U8 display driver");
  #else
  log_d("Using Adafruit display driver");
  #endif
  display_init();
}

bool refresh_screen() {
  char buf[40] = "";
  sprintf(buf, "NET: %s", get_network_status_string());
  display_clear();
  display_print(1, 20, buf);
  switch (really_connected) {
    case 0:
      sprintf(buf, "%s", "ONLINE");
      break;
    case 1:
      sprintf(buf, "%s", "- - - - -");
      break;
    default:
      sprintf(buf, "%s", "OFFLINE");
  }
  display_print(1, 30, ip_address);
  display_print_size(1,1, 2, buf);
  display_flush();
  return true;
}

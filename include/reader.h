#pragma once

#include <MFRC522.h>
#include <SPI.h>
#include <arduino-timer.h>

#include "buzzer.h"
#include "network.h"
#include "bluetooth.h"

#ifndef SS_PIN
#define SS_PIN    21
#endif
#ifndef RST_PIN
#define RST_PIN   22
#endif
#ifndef MISO_PIN
#define MISO_PIN 19
#endif
#ifndef MOSI_PIN
#define MOSI_PIN 23
#endif
#ifndef CLK_PIN
#define CLK_PIN 18
#endif
#define CYCLES_BEFORE_RESET 2

bool reader_reset(void *);
void initializeReader();
bool newCardDetected();
void bt_log(char * buf);
void processReader();
void set_mode_tap_in();
void set_mode_tap_out();
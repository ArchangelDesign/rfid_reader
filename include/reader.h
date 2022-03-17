#pragma once

#include <MFRC522.h>
#include <SPI.h>
#include <arduino-timer.h>

#include "buzzer.h"
#include "network.h"
#include "bluetooth.h"

#define CYCLES_BEFORE_RESET 2

bool reader_reset(void *);
void initializeReader();
bool newCardDetected();
void bt_log(char * buf);
void processReader();
void set_mode_tap_in();
void set_mode_tap_out();
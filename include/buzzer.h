#pragma once
#include <Arduino.h>

#ifndef BUZZER_PIN
#define BUZZER_PIN 15
#endif

void buzzer_1_beep();
void buzzer_short_beep();
void buzzer_long_beep();
void buzzer_short_beeps(int times);
void buzzer_long_beeps(int times);
void buzzer_beep(int times);
void init_buzzer();
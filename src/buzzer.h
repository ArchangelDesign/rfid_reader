#pragma once
#include <Arduino.h>

#define BUZZER_PIN 15

void buzzer_1_beep()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(30);
  digitalWrite(BUZZER_PIN, LOW);
  delay(30);
}

void buzzer_short_beep()
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1);
}

void buzzer_short_beeps(int times)
{
  for (int i = 0; i < times; i++)
    buzzer_short_beep();
}

void buzzer_beep(int times)
{
  for (int i = 0; i < times; i++)
    buzzer_1_beep();
}

void init_buzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    buzzer_beep(2);
}

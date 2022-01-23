#include <Arduino.h>
#include "reader.h"

void setup() {
  Serial.begin(9600);
  initializeReader();
  init_buzzer();
}

void loop() {
  processReader();
}
#include <Arduino.h>
#include "reader.h"
#include "screen.h"
#include "status.h"

void setup() {
  init_buzzer();
  Serial.begin(9600);
  log_d("initializing screen...");
  init_display();
  log_d("initializing network...");
  initialize_network();
  log_d("initializing reader...");
  initializeReader();
  log_d("initialization done.");
}

void loop() {
  processReader();
  process_network();
  refresh_screen();
}
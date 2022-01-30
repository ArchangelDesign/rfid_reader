#include <Arduino.h>
#include "status.h"
#include "reader.h"
#include "screen.h"
#if BT_ENABLED
#include "bluetooth.h"
#endif


void setup() {
  init_buzzer();
  Serial.begin(9600);
  log_d("initializing screen...");
  init_display();
  log_d("initializing network...");
  initialize_network();
  log_d("initializing reader...");
  initializeReader();
  #if BT_ENABLED
  log_d("initializing bluetooth...");
  initialize_blueetooth();
  #endif
  log_d("initialization done.");
}

void loop() {
  processReader();
  process_network();
  refresh_screen();
  #if BT_ENABLED
  bt_process();
  #endif
}
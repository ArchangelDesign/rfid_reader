#include <Arduino.h>
#include "status.h"
#include "storage.h"
#include "reader.h"
#include "bluetooth.h"
#include "screen.h"

void setup() {
  init_buzzer();
  Serial.begin(9600);
  initialize_eeprom();
  sleep(1);
  log_d("initializing screen...");
  init_display();
  display_print_size(100, 0, 2, "D");
  display_flush();
  sleep(1);
  display_print(115, 0, "N");
  display_flush();
  log_d("initializing network...");
  initialize_network();
  sleep(1);
  log_d("initializing reader...");
  initializeReader();
  initialize_network();
  display_print(115, 9, "R");
  display_flush();
  sleep(1);
  log_d("initializing bluetooth...");
  display_print(122, 9, "B");
  display_flush();
  sleep(1);
  initialize_blueetooth();
  log_d("initialization done.");
  display_clear();
  display_flush();
}

void loop() {
  processReader();
  process_network();
  refresh_screen();
  bt_process();
}
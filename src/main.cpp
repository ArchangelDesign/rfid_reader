#include <Arduino.h>
#include "status.h"
#include "storage.h"
#include "reader.h"
#include "bluetooth.h"
#include "screen.h"
#include "ADBuzzer.h"

ADBuzzer ad_buzzer;
bool buzzer_process_running = true;

void process_buzzer(void *params) {
  delay(100);
  while (buzzer_process_running) {
    ad_buzzer.loop();
    delay(10);
  }
  vTaskDelete(NULL);
}

void process_network(void *params) {
  delay(48);
  while (true) {
    process_network_async();
    delay(100);
  }
}

// c1 = 8717 2/6/22
// c2 = 21034 2/7/22   12317 scans
// c3 = 32853 2/9/22   11819 scans
void setup() {
  ad_buzzer.begin(BUZZER_PIN);
  xTaskCreate(process_buzzer, "buzzer_loop", 1024, NULL, 1, NULL);
  Serial.begin(9600);
  initialize_eeprom();
  ad_buzzer.beep(2);
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
  // xTaskCreate(process_network, "network_loop", 2048, NULL, 1, NULL);
}

void loop() {
  processReader();
  process_network();
  refresh_screen();
  bt_process();
}
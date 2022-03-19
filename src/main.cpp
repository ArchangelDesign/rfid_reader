#include <Arduino.h>
#include "status.h"
#include "storage.h"
#include "reader.h"
#include "screen.h"
#include "bluetooth.h"
#include "ADBuzzer.h"
#include "GtDisplay.h"
#include "GtStorage.h"

ADBuzzer ad_buzzer;
bool buzzer_process_running = true;
GtDisplay gt_display = GtDisplay();
GtStorage gt_storage = GtStorage();

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
  // xTaskCreate(process_buzzer, "buzzer_loop", 1024, NULL, 1, NULL);
  Serial.begin(115200);
  initialize_eeprom();
  ad_buzzer.beep(2);
  sleep(1);
  

  init_display();
  display_print_size(100, 0, 2, "D");
  display_flush();
  ad_buzzer.beep(1);
  sleep(1);
  display_print(115, 0, "N");
  display_flush();
  log_d("initializing network...");
  initialize_network();
  ad_buzzer.beep(1);
  sleep(1);
  log_d("initializing reader...");
  initializeReader();

  // log_d("initializing screen...");
  // gt_display.begin();

  display_print(115, 9, "R");
  display_flush();
  ad_buzzer.beep(1);
  sleep(1);
  log_d("initializing bluetooth...");
  display_print(122, 9, "B");
  display_flush();
  ad_buzzer.beep(1);
  sleep(1);
  initialize_blueetooth();
  log_d("initialization done.");
  display_clear();
  display_flush();
  if (gt_storage.begin(5, 4)) {
    delay(1000);
    gt_storage.end();
  }
  pinMode(2, OUTPUT);
  // xTaskCreate(process_network, "network_loop", 2048, NULL, 1, NULL);
}

void loop() {
  digitalWrite(2, HIGH);
  processReader();
  process_network();
  bt_process();
  refresh_screen();
  if (!gt_storage.isReady()) {
    if (gt_storage.isPresent()) {
      log_d("card present. Trying to read...");
      gt_storage.begin(5, 4);
      delay(2000);
      gt_storage.end();
    }
  }
  digitalWrite(2, LOW);
}
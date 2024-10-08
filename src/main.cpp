#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "status.h"
#include "storage.h"
#include "reader.h"
#include "bluetooth.h"
#include "ADBuzzer.h"
#include "GtDisplay.h"
#include "GtStorage.h"
// #include "SinWaveGenerator.h"
#include "GtSound.h"


ADBuzzer ad_buzzer;
bool buzzer_process_running = true;
GtStorage gt_storage = GtStorage();
GtDisplay gt_display = GtDisplay(&gt_storage);
GtSound gt_sound = GtSound(&gt_storage);
bool reported_ready = false;
bool is_system_busy = true;

// c1 = 8717 2/6/22
// c2 = 21034 2/7/22   12317 scans
// c3 = 32853 2/9/22   11819 scans
// 384 4/3/22
void setup() {
  Serial.begin(115200);
  delay(300);
  pinMode(2, OUTPUT);
  #ifndef AD_DISABLE_SOUND
  while (!gt_storage.begin(AD_SD_CS, AD_SD_CD)) {
    delay(1000);
  }
  log_d("storage initialized.");
  #else
  log_d("sound disabled");
  #endif
  // gt_sound.initialize();
  // log_d("sound initialized.");
  // sleep(1);
  // gt_sound.systemStarting();
  // sleep(2);
  initialize_eeprom();
  // gt_sound.eepromInitialized();
  // sleep(3);
  // gt_sound.shutdown();
  sleep(1);
  log_d("initializing network...");
  initialize_network();
  sleep(4);
  gt_sound.initialize();
  // delay(1000);
  // gt_sound.networkReady();
  log_d("initializing reader...");
  sleep(1);
  initializeReader();
  sleep(1);
  // gt_sound.readerInitialized();
  // sleep(3);
  log_d("initializing screen...");
  gt_display.begin();
  sleep(1);
  log_d("initializing bluetooth...");
  initialize_blueetooth();
  sleep(2);
  // gt_sound.bluetoothReady();
  sleep(3);
  
  log_d("initialization done.");
}

void loop() {
  if (really_connected && !reported_ready) {
    gt_sound.systemReady();
    sleep(3);
    is_system_busy = false;
    reported_ready = true;
  }
  processReader();
  process_network();
  bt_process();
  gt_display.loop();
}
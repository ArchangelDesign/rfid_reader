#include <Arduino.h>

#include "status.h"
#include "storage.h"
#include "reader.h"
#include "screen.h"
#include "bluetooth.h"
#include "ADBuzzer.h"
#include "GtDisplay.h"
#include "GtStorage.h"
#include "DACOutput.h"
#include "WAVFileReader.h"
#include "SinWaveGenerator.h"
#include "GtSound.h"

ADBuzzer ad_buzzer;
bool buzzer_process_running = true;
GtDisplay gt_display = GtDisplay();
GtStorage gt_storage = GtStorage();
GtSound gt_sound = GtSound();
bool reported_ready = false;

// c1 = 8717 2/6/22
// c2 = 21034 2/7/22   12317 scans
// c3 = 32853 2/9/22   11819 scans
void setup() {
  Serial.begin(115200);
  delay(300);
  while (!gt_storage.begin(AD_SD_CS, AD_SD_CD)) {
    delay(1000);
  }
  log_d("storage initialized.");
  gt_sound.initialize();
  log_d("sound initialized.");
  delay(200);
  gt_sound.systemStarting();
  delay(300);
  initialize_eeprom();
  gt_sound.eepromInitialized();
  sleep(1);
  log_d("initializing network...");
  initialize_network();
  gt_sound.networkReady();
  log_d("initializing reader...");
  sleep(1);
  initializeReader();
  gt_sound.readerInitialized();
  log_d("initializing screen...");
  gt_display.begin();
  sleep(1);
  log_d("initializing bluetooth...");
  initialize_blueetooth();
  gt_sound.bluetoothReady();
  sleep(1);
  pinMode(2, OUTPUT);
  log_d("initialization done.");
}

void loop() {
  if (really_connected && !reported_ready) {
    gt_sound.systemReady();
    reported_ready = true;
  }
  if (!really_connected) {
    gt_sound.connecting();
    sleep(3);
  }
  processReader();
  process_network();
  bt_process();
}
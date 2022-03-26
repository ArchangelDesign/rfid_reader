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

// c1 = 8717 2/6/22
// c2 = 21034 2/7/22   12317 scans
// c3 = 32853 2/9/22   11819 scans
void setup() {
  Serial.begin(115200);
  while (!gt_storage.begin(AD_SD_CS, AD_SD_CD)) {
    delay(1000);
    // gt_storage.end();
  }
  log_d("storage initialized.");
  gt_sound.initialize();
  log_d("sound initialized.");
  delay(200);
  gt_sound.systemStarting();
  initialize_eeprom();
  sleep(1);
  gt_sound.eepromInitialized();
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
  digitalWrite(2, HIGH);
  processReader();
  process_network();
  bt_process();
  gt_sound.systemReady();
  sleep(5);
  // refresh_screen();
  // if (!gt_storage.isReady()) {
  //   if (gt_storage.isPresent()) {
  //     log_d("card present. Trying to read...");
  //     gt_storage.begin(5, 4);
  //     delay(2000);
  //     gt_storage.end();
  //   }
  // }
  // digitalWrite(2, LOW);
    // delay(10);
}
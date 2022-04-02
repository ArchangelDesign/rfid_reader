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
#include "DACOutput.h"
#include "WAVFileReader.h"
#include "SinWaveGenerator.h"
#include "GtSound.h"


#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);


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
  pinMode(2, OUTPUT);
  // SPI.begin();
  // SPI.setFrequency()
  while (!gt_storage.begin(AD_SD_CS, AD_SD_CD)) {
    delay(1000);
  }
  log_d("storage initialized.");
  gt_sound.initialize();
  log_d("sound initialized.");
  sleep(1);
  gt_sound.systemStarting();
  sleep(2);
  initialize_eeprom();
  gt_sound.eepromInitialized();
  sleep(3);
  gt_sound.shutdown();
  sleep(1);
  log_d("initializing network...");
  initialize_network();
  sleep(4);
  gt_sound.initialize();
  delay(300);
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
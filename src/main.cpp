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

ADBuzzer ad_buzzer;
bool buzzer_process_running = true;
GtDisplay gt_display = GtDisplay();
GtStorage gt_storage = GtStorage();
int SineValues[256];
uint8_t dac_value = 0;
SampleSource *sampleSource;
DACOutput *output;

void process_buzzer(void *params) {
  delay(100);
  // dac_output_enable(DAC_CHANNEL_1);
  while (true) {
    for(int i=0;i<256;i++) {
      // dacWrite(25,SineValues[i]);
      // dac_output_voltage(DAC_CHANNEL_1, SineValues[i]);
    }
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
  

  // init_display();
  // display_print_size(100, 0, 2, "D");
  // display_flush();
  ad_buzzer.beep(1);
  sleep(1);
  // display_print(115, 0, "N");
  // display_flush();
  log_d("initializing network...");
  initialize_network();
  ad_buzzer.beep(1);
  sleep(1);
  log_d("initializing reader...");
  initializeReader();

  log_d("initializing screen...");
  gt_display.begin();

  // display_print(115, 9, "R");
  // display_flush();
  ad_buzzer.beep(1);
  sleep(1);
  log_d("initializing bluetooth...");
  // display_print(122, 9, "B");
  // display_flush();
  ad_buzzer.beep(1);
  sleep(1);
  initialize_blueetooth();
  log_d("initialization done.");
  // display_clear();
  // display_flush();
  while (!gt_storage.begin(AD_SD_CS, AD_SD_CD)) {
    delay(1000);
    // gt_storage.end();
  }
  pinMode(2, OUTPUT);
  // xTaskCreate(process_network, "network_loop", 2048, NULL, 1, NULL);

    float ConversionFactor=(2*PI)/256;        // convert my 0-255 bits in a circle to radians
                                            // there are 2 x PI radians in a circle hence the 2*PI
                                            // Then divide by 256 to get the value in radians
                                            // for one of my 0-255 bits.
  float RadAngle;                           // Angle in Radians
  // calculate sine values
  for(int MyAngle=0;MyAngle<256;MyAngle++) {
    RadAngle=MyAngle*ConversionFactor;               // 8 bit angle converted to radians
    SineValues[MyAngle]=(sin(RadAngle)*127)+128;  // get the sine of this angle and 'shift' up so
                                            // there are no negative values in the data
                                            // as the DAC does not understand them and would
                                            // convert to positive values.
  }

  sampleSource = new WAVFileReader("/sample.wav");
  // sampleSource = new SinWaveGenerator(40000, 600, 0.75);
  output = new DACOutput();
  output->start(sampleSource);
}

void loop() {
  digitalWrite(2, HIGH);
  processReader();
  process_network();
  bt_process();
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
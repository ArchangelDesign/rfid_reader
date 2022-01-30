#include "reader.h"
#include "status.h"
#include "screen.h"

MFRC522::MIFARE_Key key = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
MFRC522::StatusCode status;
MFRC522 mfrc522(SS_PIN, RST_PIN);
byte cardId[4] = {0, 0, 0, 0};
Timer<1, millis> reader_timer;
uint8_t reset_cycles = 0;
gt_mode_t current_mode = gt_tap_in;
char last_error[50];

bool reader_reset(void *)
{
    if (reset_cycles > CYCLES_BEFORE_RESET) {
        reset_cycles = 0;
        memset(cardId, 0, 4);
        memset(last_error, 0, 50);
    }
    reset_cycles++;
    return true;
}

void initializeReader() {
    SPI.begin(CLK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
    mfrc522.PCD_Init();
    reader_timer.every(1000, reader_reset);
}

bool newCardDetected() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return false;
  }

  if (memcmp(cardId, mfrc522.uid.uidByte, 4) == 0) {
    return false;
  }

  return true;
}

void bt_log(char * buf) {
  #if BT_ENABLED
  bt_send(buf);
  bt_send("\n");
  #endif
}

void processReader()
{
    reader_timer.tick();
    if (!newCardDetected())
        return;
    memcpy(cardId, mfrc522.uid.uidByte, 4);
    reset_cycles = 0;
    Serial.printf("Card ID: %x:%x:%x:%x\n", cardId[0], cardId[1], cardId[2], cardId[3]);
    char buf[60];
    sprintf(buf, "Card ID: %x:%x:%x:%x", cardId[0], cardId[1], cardId[2], cardId[3]);
    sprintf(last_error, "%s", "BUSY");
    refresh_screen();
    bt_log(buf);
    buzzer_beep(1);
    uint8_t result = tap_in(cardId);
    switch (result) {
      case ACTION_OK:
        bt_log("OK");
        sprintf(last_error, "%s", "OK");
        refresh_screen();
        buzzer_beep(2);
      break;
      case ERR_INVALID_UID:
        bt_log("INVALID CARD");
        sprintf(last_error, "%s", "INVALID");
        refresh_screen();
        buzzer_long_beeps(2);
        break;
      case ERR_ALREADY_TAPPED_IN:
      case ERR_ALREADY_TAPPED_OUT:
        bt_log("INVALID STATE");
        sprintf(last_error, "%s", "STATE");
        refresh_screen();
        buzzer_long_beeps(1);
        break;
      case ERR_SERVER_ERROR:
        sprintf(last_error, "%s", "SERVER FAILURE");
        refresh_screen();
        bt_log("SERVER ERROR");
        buzzer_long_beeps(2);
    }
}

void set_mode_tap_in() {
  log_d("switching to tap in mode...");
  current_mode = gt_tap_in;
}

void set_mode_tap_out() {
  log_d("switching to tap out mode...");
  current_mode = gt_tap_out;
}
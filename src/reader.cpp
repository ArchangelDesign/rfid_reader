#include "reader.h"
#include "status.h"
#include "storage.h"
#include "GtSound.h"

MFRC522::MIFARE_Key key = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
MFRC522::StatusCode status;
MFRC522 mfrc522(AD_SS_PIN, AD_RST_PIN);
byte cardId[4] = {0, 0, 0, 0};
Timer<1, millis> reader_timer;
uint8_t reset_cycles = 0;
gt_mode_t current_mode = gt_tap_in;
char last_error[50];

uint32_t scan_counter;
uint32_t scan_counter_ok;

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
  SPI.begin();
  scan_counter = gt_mem_get_cntr();
  
  mfrc522.PCD_Init();
  reader_timer.every(1000, reader_reset);
}

bool newCardDetected() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  if (memcmp(cardId, mfrc522.uid.uidByte, 4) == 0) {
    return false;
  }

  return true;
}

void bt_log(const char * buf) {
  bt_send(buf);
  bt_send("\n");
}

void processReader()
{
    reader_timer.tick();
    if (!newCardDetected() || is_system_busy) {
        return;
    }
    is_system_busy = true;
    memcpy(cardId, mfrc522.uid.uidByte, 4);
    reset_cycles = 0;
    Serial.printf("Card ID: %x:%x:%x:%x\n", cardId[0], cardId[1], cardId[2], cardId[3]);
    char buf[60];
    sprintf(buf, "Card ID: %x:%x:%x:%x", cardId[0], cardId[1], cardId[2], cardId[3]);
    sprintf(last_error, "%s", "BUSY");
    bt_log((const char*)buf);
    gt_sound.cardDetected();
    delay(1000);
    uint8_t result = 90;
    if (current_mode == gt_tap_in) {
      result = tap_in(cardId);
    }
    if (current_mode == gt_tap_out) {
      result = tap_out(cardId);
    }
    switch (result) {
      case ACTION_OK:
        bt_log("OK");
        gt_sound.ok();
        scan_counter_ok++;
        break;
      case ERR_INVALID_UID:
        bt_log("INVALID CARD");
        sprintf(last_error, "%s", "INVALID");
        gt_sound.invalidCard();
        break;
      case ERR_ALREADY_TAPPED_IN:
      case ERR_ALREADY_TAPPED_OUT:
        bt_log("INVALID STATE");
        sprintf(last_error, "%s", "STATE");
        break;
      case ERR_SERVER_ERROR:
        sprintf(last_error, "%s", "SERVER FAILURE");
        bt_log("SERVER ERROR");
        gt_sound.networkError();
        break;
      case ERR_NO_CONNECTION:
        sprintf(last_error, "%s", "NO NET");
        bt_log("NO CONNECTION");
        gt_sound.networkError();
        break;
      default:
        sprintf(last_error, "%s", "ERROR");
        bt_log("INVALID CODE RETURNED FROM THE SERVER");
        gt_sound.networkError();
        break;
    }
    scan_counter++;
    Serial.printf("SCANS: %d\n", scan_counter);
    gt_mem_set_cntr(scan_counter);
    sleep(2);
    is_system_busy = false;
}

void set_mode_tap_in() {
  log_d("switching to tap in mode...");
  current_mode = gt_tap_in;
}

void set_mode_tap_out() {
  log_d("switching to tap out mode...");
  current_mode = gt_tap_out;
}
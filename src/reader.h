#pragma once

#include <MFRC522.h>
#include <SPI.h>
#include <arduino-timer.h>

#include "buzzer.h"

#define SS_PIN    21
#define RST_PIN   22
#define CYCLES_BEFORE_RESET 2

MFRC522::MIFARE_Key key = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
MFRC522::StatusCode status;
MFRC522 mfrc522(SS_PIN, RST_PIN);
byte cardId[4] = {0, 0, 0, 0};
Timer<1, millis> reader_timer;
uint8_t reset_cycles = 0;

bool reader_reset(void *)
{
    if (reset_cycles > CYCLES_BEFORE_RESET) {
        reset_cycles = 0;
        memset(cardId, 0, 4);
    }
    reset_cycles++;
    return true;
}

void initializeReader() {
    SPI.begin();
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

void processReader()
{
    reader_timer.tick();
    if (!newCardDetected())
        return;
    memcpy(cardId, mfrc522.uid.uidByte, 4);
    reset_cycles = 0;
    Serial.printf("Card ID: %x:%x:%x:%x\n", cardId[0], cardId[1], cardId[2], cardId[3]);
    buzzer_1_beep();
}
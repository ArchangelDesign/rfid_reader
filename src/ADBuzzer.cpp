#include "ADBuzzer.h"

void ADBuzzer::begin(uint8_t pin, uint16_t signalLength, uint16_t pauseLength, uint16_t longSignalLength, uint16_t longPauseLength) {
    this->pin = pin;
    this->signalLength = signalLength;
    this->pauseLength = pauseLength;
    this->longSignalLength = longSignalLength;
    this->longPauseLength = longPauseLength;
    pinMode(this->pin, OUTPUT);
}

void ADBuzzer::beep(uint8_t times) {
    this->beepsToGo += times;
}

void ADBuzzer::longBeep(uint8_t times) {
    this->longBeepsToGo += times;
}

void ADBuzzer::beepShort() {
    digitalWrite(this->pin, HIGH);
    delay(this->signalLength);
    digitalWrite(this->pin, LOW);
    delay(this->pauseLength);
}

void ADBuzzer::beepLong() {
    digitalWrite(this->pin, HIGH);
    delay(this->longSignalLength);
    digitalWrite(this->pin, LOW);
    delay(this->longPauseLength);
}

void ADBuzzer::loop() {
    if (this->longBeepsToGo > 0) {
        this->beepLong();
        this->longBeepsToGo--;
    }
    if (this->beepsToGo == 0) {
        return;
    }
    this->beepShort();
    this->beepsToGo--;
}

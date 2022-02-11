#include "ADBuzzer.h"

ADBuzzer *adBuzzerInstance = NULL;

ADBuzzer::ADBuzzer(uint8_t pin, uint16_t signalLength, uint16_t pauseLength) {
    this->pin = pin;
    this->signalLength = signalLength;
    this->pauseLength = pauseLength;
    pinMode(this->pin, OUTPUT);
}

void ADBuzzer::beep() {
    this->beepsToGo++;
}

void ADBuzzer::beepShort(uint8_t times) {
    digitalWrite(this->pin, HIGH);
    delay(this->signalLength);
    digitalWrite(this->pin, LOW);
    delay(this->pauseLength);
}

void ADBuzzer::beepShortAsync(uint8_t times) {
    this->beepsToGo += times;
}

void ADBuzzer::loop() {
    if (this->beepsToGo == 0) {
        return;
    }
    this->beepShort(1);
    this->beepsToGo--;
}

#pragma once

#include <Arduino.h>

class ADBuzzer {
    public:
        void begin(uint8_t pin, uint16_t signalLength = 50, uint16_t pauseLength = 50, uint16_t longSignalLength = 100, uint16_t longPauseLength = 100);
        void beep(uint8_t times = 1);
        void longBeep(uint8_t times = 1);
        void loop();

    private:
        uint8_t pin;
        uint16_t signalLength;
        uint16_t pauseLength;
        uint16_t longSignalLength;
        uint16_t longPauseLength;
        uint8_t beepsToGo = 0;
        uint8_t longBeepsToGo = 0;
        void beepShort();
        void beepLong();
        void beepShortAsync(uint8_t times);
};
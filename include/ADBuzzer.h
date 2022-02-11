#pragma once

#include <Arduino.h>

class ADBuzzer {
    public:
        ADBuzzer(uint8_t pin, uint16_t signalLength = 50, uint16_t pauseLength = 50);
        void beep();
        void loop();

    private:
        uint8_t pin;
        uint16_t signalLength;
        uint16_t pauseLength;
        uint8_t beepsToGo = 0;
        void beepShort(uint8_t times);
        void beepShortAsync(uint8_t times);
};
#ifndef __GT_STORAGE_H__
#define __GT_STORAGE_H__

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 36
#define SD_MISO 34 // DO
#define SD_MOSI 39 // DI
#define SD_CLK 35

#define AD_VOLUME_INFO_FILE "volume.inf"

class GtStorage {
    public:
        bool begin(uint8_t cs, uint8_t cd);
        uint32_t getCardSize();
        bool isPresent();
        bool isReady();
        uint8_t getFatType();
        void openRoot();
        bool writeInfo(const char *info);
    private:
    #ifdef AD_AVR
        Sd2Card card;
        SdVolume volume;
        SdFile root;
    #endif
        uint8_t chipSelectPin;
        uint8_t cardDetectionPin;
        bool initialized = false;

        bool initialize();
};

#endif
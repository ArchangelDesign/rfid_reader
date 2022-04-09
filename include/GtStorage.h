#ifndef __GT_STORAGE_H__
#define __GT_STORAGE_H__

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define AD_VOLUME_INFO_FILE "volume.inf"

class GtStorage {
    public:
        bool begin(uint8_t cs, int8_t cd = -1);
        uint32_t getCardSize();
        uint32_t getCardSizeGb();
        bool isPresent();
        bool isReady();
        uint8_t getFatType();
        const char* getCardTypeString();
        void openRoot();
        bool writeInfo(const char *info);
        void end();
    private:
    #ifdef AD_AVR
        Sd2Card card;
        SdVolume volume;
        SdFile root;
    #endif
        uint8_t chipSelectPin;
        int8_t cardDetectionPin;
        bool initialized = false;
        bool initialize();
};

#endif
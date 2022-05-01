#pragma once
#include "I2SOutput.h"
#include "WAVFileReader.h"
#include "GtStorage.h"

class GtSound {
    public:
        GtSound(GtStorage *gt_storage);
        void systemStarting();
        void networkReady();
        void bluetoothReady();
        void systemReady();
        void readerInitialized();
        void eepromInitialized();
        void connectedToWiFi();
        void initialize();
        void shutdown();
        void disconnectedFromServer();
        void connecting();
        void cardDetected();
        void invalidCard();
        void ok();
        void networkError();
        WAVFileReader* loadSound(const char* fname);
    private:
        I2SOutput *output;
        GtStorage *storage;
};

extern GtSound gt_sound;
#pragma once
#include "DACOutput.h"

class GtSound {
    public:
        GtSound();
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
    private:
        DACOutput *output;
};

extern GtSound gt_sound;
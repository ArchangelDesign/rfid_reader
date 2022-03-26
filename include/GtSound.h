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
        void initialize();
    private:
        DACOutput *output;
};
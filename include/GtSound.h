#pragma once
#include "DACOutput.h"

class GtSound {
    public:
        GtSound();
        void systemStarting();
        void networkReady();
        void bluetoothReady();
        void systemReady();
    private:
        DACOutput *output;
};
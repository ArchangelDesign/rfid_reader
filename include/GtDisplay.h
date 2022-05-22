#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Wire.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include "GtStorage.h"

class GtDisplay {
    public:
        GtDisplay(GtStorage *storage);
        void begin();
        void loop();
        void mati(int cycle);
    private:
        void paint();
        void clear();
        void printLine1(char buf[]);
        void drawGrid();
        void drawLogo();
        void drawWifiInfo();
        void drawSdInfo();
        void drawIpAddress();
        void setFontRegularBackgroundColor();
        void setFontTitle();
        void setFontRegular();
        void drawScanCounter();
        void drawBluetoothInfo();
        void drawVersion();
        void drawStatus();

        uint16_t frameCount = 0;
        Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
        char buffer[30];
        GtStorage *storage;
        char* lastIpAddress = "";
        uint32_t lastScanCount = 0;
        gt_mode_t lastMode;
};
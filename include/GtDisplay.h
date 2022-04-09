#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Wire.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "GtStorage.h"

#define TFT_CS        33 // yellow
#define TFT_RST       27 // brown
#define TFT_DC        32 // blue

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
        void drawSdInfo();
        void setFontTitle();
        void setFontRegular();

        uint16_t frameCount = 0;
        Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
        char buffer[30];
        GtStorage *storage;
};
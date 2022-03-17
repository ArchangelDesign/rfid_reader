#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Wire.h>
#include <Fonts/FreeMonoBold18pt7b.h>

#define TFT_BACKLIGHT 22
#define TFT_CS        34 // yellow
#define TFT_RST       35 // brown
#define TFT_DC        32 // blue

class GtDisplay {
    public:
        GtDisplay();
        void begin();
        void loop();
        void mati(int cycle);
    private:
        void paint();
        void clear();
        void printLine1(char buf[]);
        uint16_t frameCount = 0;
        Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
        char buffer[30];
};
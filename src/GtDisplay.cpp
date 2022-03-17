
#include "GtDisplay.h"


GtDisplay::GtDisplay() {
    memset(buffer, 0, 30);
}

void GtDisplay::begin() {
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH);
    tft.init(240, 320);
    tft.setRotation(3);
    tft.setFont(&FreeMonoBold18pt7b);
    delay(100);
    
    tft.fillScreen(ST77XX_CYAN);
    tft.fillRect(0, 0, 320, 50, ST77XX_BLACK);
    paint();
}

void GtDisplay::paint() {
    char buf[30] = {};
    sprintf(buf, "FRAME: %d", frameCount);
    printLine1(buf);
    frameCount++;
}

void GtDisplay::printLine1(char buf[]) {
    int index = 0;
    while (buf[index] != 0 && index < 30) {
        if (buffer[index] == buf[index]) {
            buffer[index] = ' ';
        }
        index++;
    }
    tft.setCursor(5, 35);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(buffer);
    delay(5);
    tft.setCursor(5, 35);
    tft.setTextColor(ST77XX_WHITE);
    tft.print(buf);
    memcpy(buffer, buf, 30);
}

void GtDisplay::loop() {
    paint();
    // delay(300);
}
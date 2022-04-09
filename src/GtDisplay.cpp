#include "GtDisplay.h"
#include "network.h"

GtDisplay::GtDisplay(GtStorage *storeage) {
    memset(buffer, 0, 30);
    this->storage = storage;
}

void GtDisplay::begin() {
    tft.init(240, 320);
    tft.setRotation(3);
    tft.setFont(&FreeMonoBold18pt7b);
    delay(100);
    clear();
    drawGrid();
    paint();
}

void GtDisplay::paint() {

}

void GtDisplay::loop() {
    paint();
    // delay(300);
}

void GtDisplay::clear()
{
    tft.fillScreen(ST77XX_CYAN);
}

void GtDisplay::drawGrid()
{
    drawSdInfo();
    drawLogo();
}

void GtDisplay::drawSdInfo()
{
    tft.drawRoundRect(1, 175, 160, 64, 5, ST77XX_BLACK);
    tft.setCursor(18, 180);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(1);
    setFontTitle();
    tft.fillRect(10, 170, 40, 10, ST77XX_CYAN);
    tft.print("SD");
    tft.setCursor(8, 200);
    setFontRegular();
    tft.printf("Size: %d GB", storage->getCardSizeGb());
    tft.setCursor(8, 225);
    tft.printf("Type: %s", storage->getCardTypeString());
}

void GtDisplay::drawLogo()
{

}

void GtDisplay::setFontTitle()
{
    tft.setFont(&FreeSansBold9pt7b);
    tft.setTextSize(1);
}

void GtDisplay::setFontRegular()
{
    tft.setFont(&FreeSans9pt7b);
    tft.setTextSize(1);
}
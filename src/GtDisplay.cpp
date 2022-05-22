#include "GtDisplay.h"
#include "status.h"

extern GtStorage gt_storage;

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
    lastMode = current_mode;
}

void GtDisplay::paint() {

}

void GtDisplay::loop() {
    if (gt_storage.isBusy()) {
        Serial.println("STORAGE IS BUSY");
        return;
    }
    if (strcmp(lastIpAddress, ip_address) != 0) {
        drawIpAddress();
    }
    if (scan_counter > lastScanCount) {
        drawScanCounter();
    }
    if (lastMode != current_mode) {
        drawStatus();
    }
}

void GtDisplay::clear()
{
    tft.fillScreen(ST77XX_CYAN);
}

void GtDisplay::drawGrid()
{
    drawSdInfo();
    drawWifiInfo();
    drawLogo();
    drawScanCounter();
    drawBluetoothInfo();
    drawVersion();
    drawStatus();
}

void GtDisplay::drawSdInfo()
{
    tft.drawRoundRect(1, 175, 140, 64, 5, ST77XX_BLACK);
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

void GtDisplay::drawWifiInfo()
{
    tft.drawRoundRect(148, 175, 170, 64, 5, ST77XX_BLACK);
    tft.setCursor(165, 180);
    tft.setTextColor(ST77XX_BLACK);
    setFontTitle();
    tft.fillRect(160, 170, 55, 10, ST77XX_CYAN);
    tft.print("WiFi");
    tft.setCursor(160, 200);
    setFontRegular();
    tft.printf("%s", gt_ssid);
}

void GtDisplay::drawIpAddress()
{
    tft.setCursor(160, 225);
    setFontRegularBackgroundColor();
    tft.printf("%s", lastIpAddress);
    setFontRegular();
    tft.setCursor(160, 225);
    lastIpAddress = ip_address;
    tft.printf("%s", lastIpAddress);
}

void GtDisplay::drawLogo()
{
    tft.setFont(&FreeSans18pt7b);
    tft.setCursor(5, 40);
    tft.setTextColor(ST77XX_BLACK);
    tft.print("grow");
    tft.setFont(&FreeSansBold18pt7b);
    tft.print("tix");
    uint8_t offset = 94;
    uint8_t y = 19;
    uint8_t distance = 2;
    uint8_t w = 5;
    tft.fillRect(offset, y - 4, w, 4, ST77XX_RED);
    tft.fillRect(offset + w + distance, y - 8, w, 8, ST77XX_YELLOW);
    tft.fillRect(offset + ((w + distance) * 2), y - 12, w, 12, ST77XX_GREEN);
    tft.fillRect(offset + ((w + distance) * 3), y - 15, w, 15, ST77XX_BLUE);
}

void GtDisplay::setFontTitle()
{
    tft.setFont(&FreeSansBold9pt7b);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_BLACK);
}

void GtDisplay::setFontRegular()
{
    tft.setFont(&FreeSans9pt7b);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_BLACK);
}

void GtDisplay::setFontRegularBackgroundColor()
{
    tft.setFont(&FreeSans9pt7b);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_CYAN);
}

void GtDisplay::drawScanCounter()
{
    setFontRegularBackgroundColor();
    tft.setCursor(130, 15);
    tft.printf("Scans: %d", lastScanCount);
    setFontRegular();
    tft.setCursor(130, 15);
    tft.printf("Scans: %d", scan_counter);
    lastScanCount = scan_counter;
}

void GtDisplay::drawBluetoothInfo()
{
    setFontRegular();
    tft.setCursor(130, 35);
    tft.printf("%s", STRINGIFY(BT_DEVICE_NAME));
}

void GtDisplay::drawVersion()
{
    setFontRegular();
    tft.setCursor(260, 15);
    tft.printf("%s", STRINGIFY(GT_VERSION));
}

void GtDisplay::drawStatus()
{
    tft.fillRect(0, 60, 320, 80, ST77XX_GREEN);
    tft.setCursor(10, 110);
    tft.setFont(&FreeMonoBold24pt7b);
    switch (current_mode) {
        case gt_tap_in:
            tft.print("TAP IN");
            break;
        case gt_tap_out:
            tft.print("TAP OUT");
            break;
    }

}
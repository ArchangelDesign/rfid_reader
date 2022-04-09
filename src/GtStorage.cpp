#include "GtStorage.h"

bool GtStorage::begin(uint8_t cs, int8_t cd) {
    this->chipSelectPin = cs;
    this->cardDetectionPin = cd;
    if (cardDetectionPin > -1) {
        pinMode(cardDetectionPin, INPUT_PULLUP);
    }
    initialized = false;

    if (!isPresent()) {
        log_e("No card present.");
        return false;
    }
    initialized = initialize();
    return initialized;
}

void GtStorage::end() {
    SD.end();
}

bool GtStorage::initialize() {
    if (!SD.begin(chipSelectPin)) {
        Serial.println("cannot mount.");
        return false;
    }
    Serial.println("mounted!");
    Serial.print("Card Type: ");
    Serial.println(getCardTypeString());
    return true;
}

const char* GtStorage::getCardTypeString()
{
    switch (SD.cardType()) {
        case CARD_SD:
            return "SDSC";
        case CARD_MMC:
            return "MMC";
        case CARD_SDHC:
            return "SDHC";
        default:
            return "UNKNOWN";
    }
}

uint32_t GtStorage::getCardSize() {
    return SD.cardSize() / 1024 / 1024;
}

uint32_t GtStorage::getCardSizeGb() {
    return SD.cardSize() / 1024 / 1024 / 1024;
}

bool GtStorage::isPresent() {
    if (cardDetectionPin == -1) {
        return true;
    }
    return digitalRead(cardDetectionPin) == HIGH;
}

bool GtStorage::isReady() {
    return isPresent() && initialized;
}

uint8_t GtStorage::getFatType() {
    #ifdef AD_AVR
    return volume.fatType();
    #endif
    return 0;
}

void GtStorage::openRoot() {
    #ifdef AD_AVR
    root.openRoot(volume);
    root.ls(LS_R | LS_DATE | LS_SIZE);
    #endif
}

bool GtStorage::writeInfo(const char* info) {
    #ifdef AD_AVR
    root.openRoot(volume);
    File infoFile;
    if (SD.exists(AD_VOLUME_INFO_FILE)) {
        Serial.println("info exists");
        infoFile = SD.open(AD_VOLUME_INFO_FILE, O_WRITE | O_APPEND);
    } else {
        Serial.println("First write!");
        infoFile = SD.open(AD_VOLUME_INFO_FILE, FILE_WRITE);
    }
    if (!infoFile) {
        Serial.println("cannot open file.");
        return false;
    }
    uint16_t written = infoFile.write(info);
    bool success = written == strlen(info);
    Serial.print("written: ");
    Serial.println(written);
    infoFile.close();
    return success;
    #endif
    return false;
}
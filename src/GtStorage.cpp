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
    #ifdef AD_AVR
    if (!card.init(SPI_HALF_SPEED, chipSelectPin)) {
        Serial.println("card init failed");
        delay(1000);
        return false;
    }
    Serial.print("card type: ");
    switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
    }

    if (!volume.init(card)) {
        Serial.println("Cannot mount volume");
        return false;
    }
    Serial.print("FAT count: ");
    Serial.println(volume.fatCount());
    Serial.print("Clusters: ");
    Serial.println(volume.clusterCount());

    Serial.print("SIZE: ");
    Serial.print(getCardSize());
    Serial.println(" MB");
    openRoot();
    return true;
    #endif
    #ifdef AD_ESP
    uint8_t cardType = SD.cardType();
    Serial.print("Card Type: ");
    switch (cardType) {
        case CARD_SD:
            Serial.println("SDSC");
            break;
        case CARD_MMC:
            Serial.println("MMC");
            break;
        case CARD_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("UNKNOWN");
    }
    #endif
    return true;
}

uint32_t GtStorage::getCardSize() {
    #ifdef AD_AVR
    uint32_t volumesize;
    volumesize = volume.blocksPerCluster();
    volumesize *= volume.clusterCount();
    volumesize /= 2;
    volumesize /= 1024;
    return volumesize / 1024;
    #endif
    #ifdef AD_ESP
    return SD.cardSize() / 1024 / 1024;
    #endif
    return 0;
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
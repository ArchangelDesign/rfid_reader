#include "storage.h"

void initialize_eeprom() {
    EEPROM.begin(GT_MEM_TOTAL_SIZE);
}

void gt_mem_get_ssid(char *out) {
    char buf[GT_MEM_SIZE_SSID] = "";

    for (int addr = GT_MEM_ADDR_SSID; addr < GT_MEM_SIZE_SSID; addr++) {
        int index = addr - GT_MEM_ADDR_SSID;
        buf[index] = EEPROM.read(addr);
    }

    strcpy(out, buf);
}

void gt_mem_get_pass(char *out) {
    char buf[GT_MEM_SIZE_PASS] = "";

    for (int addr = GT_MEM_ADDR_PASS; addr < GT_MEM_ADDR_PASS + GT_MEM_SIZE_PASS; addr++) {
        int index = addr - GT_MEM_ADDR_PASS;
        buf[index] = EEPROM.read(addr);
    }

    strcpy(out, buf);
}

void gt_mem_set_ssid(char *new_ssid) {
    EEPROM.writeString(GT_MEM_ADDR_SSID, new_ssid);
    EEPROM.commit();
}

void gt_mem_set_pass(char *new_pass) {
    EEPROM.writeString(GT_MEM_ADDR_PASS, new_pass);
    EEPROM.commit();
}

uint32_t gt_mem_get_cntr() {
    return EEPROM.readLong(GT_MEM_ADDR_CNTR);
}

void gt_mem_set_cntr(uint32_t new_cntr) {
    EEPROM.writeLong(GT_MEM_ADDR_CNTR, new_cntr);
    EEPROM.commit();
}
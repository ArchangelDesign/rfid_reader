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
#pragma once

#include <EEPROM.h>

#define GT_MEM_TOTAL_SIZE 128
#define GT_MEM_ADDR_SSID 0
#define GT_MEM_SIZE_SSID 40

void initialize_eeprom();
void gt_mem_get_ssid(char *out);
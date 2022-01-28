#pragma once

#include <Arduino.h>
#define FLASH_OUTPUT_BUFFER_SIZE 30

char outputString[FLASH_OUTPUT_BUFFER_SIZE];

char* flash_get(const char* str)
{
  strcpy_P(outputString, (char*)str);
  return outputString;
}

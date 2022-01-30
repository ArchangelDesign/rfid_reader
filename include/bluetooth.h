#pragma once

#include "Arduino.h"
#include "BluetoothSerial.h"

#ifndef BT_BUFFER_SIZE
#define BT_BUFFER_SIZE 120
#endif

extern BluetoothSerial SerialBT;

void initialize_blueetooth();

void bt_process();

void bt_auth_completed_callback(boolean success);

void bt_data_received(const uint8_t *buffer, size_t size);

void bt_send(char * buf);
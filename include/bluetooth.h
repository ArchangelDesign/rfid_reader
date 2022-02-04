#pragma once

#include "Arduino.h"
#include "BluetoothSerial.h"

#ifndef BT_BUFFER_SIZE
#define BT_BUFFER_SIZE 120
#endif

#ifndef BT_DEVICE_NAME
#define BT_DEVICE_NAME "GT RFID SCANNER"
#endif

extern BluetoothSerial SerialBT;

void initialize_blueetooth();

void bt_process();

void bt_auth_completed_callback(boolean success);

void bt_data_received(const uint8_t *buffer, size_t size);

void bt_send(char * buf);

void bt_request_confirmed(uint32_t num_val);
#pragma once

#include "Arduino.h"
#include "BluetoothSerial.h"
#include "esp_gap_bt_api.h"

#ifndef BT_BUFFER_SIZE
#define BT_BUFFER_SIZE 120
#endif

#ifndef BT_DEVICE_NAME
#define BT_DEVICE_NAME "GT RFID SCANNER"
#endif

enum bt_state_t {
    gt_state_normal,
    gt_state_ssid,
    gt_state_password
};

extern BluetoothSerial SerialBT;

void initialize_blueetooth();

void bt_process();

void bt_auth_completed_callback(boolean success);

void bt_data_received(const uint8_t *buffer, size_t size);

void bt_send(char * buf);

void bt_request_confirmed(uint32_t num_val);

void gt_bt_command_received();

void bt_callback (esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

void bt_gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
#pragma once

#include <Arduino.h>

typedef enum gt_mode_t {
    gt_tap_in,
    gt_tap_out
};

extern uint8_t network_status;
extern int8_t really_connected;
extern char ip_address[21];
extern gt_mode_t current_mode;
extern char last_error[50];

void set_mode_tap_out();
void set_mode_tap_in();
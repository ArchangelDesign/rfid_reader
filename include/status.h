#pragma once

#include <Arduino.h>

#define STRINGIFY(s) STRINGIFY1(s)
#define STRINGIFY1(s) #s

#ifndef GT_VERSION
#define GT_VERSION "debug"
#endif

enum gt_mode_t {
    gt_tap_in,
    gt_tap_out
};

extern uint8_t network_status;
extern bool really_connected;
extern char ip_address[21];
extern gt_mode_t current_mode;
extern char last_error[50];
extern uint32_t scan_counter;
extern uint32_t scan_counter_ok;
extern char gt_ssid[];
extern bool is_system_busy;

void set_mode_tap_out();
void set_mode_tap_in();
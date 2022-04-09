#pragma once

#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <arduino-timer.h>

#define RES_BUFFER_SIZE 8
#define REQ_BUFFER_SIZE 120

#define ERR_NO_CONNECTION 9
#define ACTION_OK 0
#define ERR_INVALID_UID 1
#define ERR_ALREADY_TAPPED_IN 2
#define ERR_ALREADY_TAPPED_OUT 3
#define ERR_SERVER_ERROR 4

#define NET_STATUS_OK 3
#define NET_STATUS_INVALID_SSID 1
#define NET_STATUS_DISCONNECTED 6
#define NET_STATUS_CONN_LOST 5
#define NET_STATUS_FAILED 4 
#define NET_STATUS_IDLE 0

extern char gt_ssid[];

const char* get_network_status_string();
uint8_t tap_in(byte uid[]);
uint8_t tap_out(byte uid[]);
void initialize_network();
bool network_status_callback(void *);
bool is_connected();
void process_network();
int post_to_endpoint(const char* url, byte uid[]);
void gt_connect();
void process_network_async();
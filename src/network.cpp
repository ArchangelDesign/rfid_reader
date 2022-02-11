#include "network.h"
#include "buzzer.h"
#include "status.h"
#include "storage.h"

uint8_t network_status = NET_STATUS_IDLE;
uint8_t disconnected_seconds = 0;
char response_body_buffer[RES_BUFFER_SIZE] = {};
char request_body_buffer[REQ_BUFFER_SIZE] = {};

Timer<3, millis> network_timer;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server_address, server_port);
int8_t really_connected = 1;
char ip_address[21] = "";
char gt_ssid[GT_MEM_SIZE_SSID];
char gt_pass[GT_MEM_SIZE_PASS];

bool is_connected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool network_status_callback(void *) {
    if (network_status != WiFi.status()) {
        log_d("Network status switched to %d", WiFi.status());
        network_status = WiFi.status();
        if (network_status == NET_STATUS_OK) {
            sprintf(ip_address, "%s", WiFi.localIP().toString().c_str());
            log_i("Connected to %s with IP address: %s", ssid,ip_address);
            buzzer_beep(5);
        }
    }
    if (!is_connected()) {
        disconnected_seconds++;
        really_connected = 128;
    }
    if (disconnected_seconds > 10) {
        buzzer_beep(5);
        Serial.println("trying to reconnect...");
        disconnected_seconds = 0;
        gt_connect();
    }
    if (is_connected()) {
        really_connected = 1;
        client.setTimeout(1000);
        client.beginRequest();
        int res = client.get(STRINGIFY(URL_TAP_IN));
        client.endRequest();
        really_connected = res;
    }
    return true;
}

void initialize_network() {
    gt_connect();
    network_timer.every(1000, network_status_callback);
}

void gt_connect() {
  buzzer_beep(1);
  memset(gt_ssid, 0, GT_MEM_SIZE_SSID);
  gt_mem_get_ssid(gt_ssid);
  memset(gt_pass, 0, GT_MEM_SIZE_PASS);
  gt_mem_get_pass(gt_pass);
  log_d("SSID from EEPROM: %s", gt_ssid);
  log_d("PASS from EEPROM: %s", gt_pass);
  WiFi.begin((const char*)gt_ssid, gt_pass);
}

uint8_t tap_in(byte uid[]) {
  
    if (!is_connected()) {
        return ERR_NO_CONNECTION;
    }
    int response_code = post_to_endpoint(STRINGIFY(URL_TAP_IN), uid);
    log_d("response code: %d", response_code);
    if (response_code != 200) {
        return ERR_SERVER_ERROR;
    }
    memset(response_body_buffer, 0, RES_BUFFER_SIZE);
    strcpy(response_body_buffer, client.responseBody().c_str());
    return response_body_buffer[0];;
}

uint8_t tap_out(byte uid[]) {
    if (!is_connected()) {
        return ERR_NO_CONNECTION;
    }
    int response_code = post_to_endpoint(STRINGIFY(URL_TAP_OUT), uid);
    log_d("response code: %d", response_code);
    if (response_code != 200) {
        return ERR_SERVER_ERROR;
    }
    memset(response_body_buffer, 0, RES_BUFFER_SIZE);
    strcpy(response_body_buffer, client.responseBody().c_str());
    return response_body_buffer[0];
}

int post_to_endpoint(const char* url, byte uid[]) {
    client.beginRequest();
    client.setTimeout(5000);
    memset(request_body_buffer, 0, REQ_BUFFER_SIZE);
    sprintf(request_body_buffer, "%x:%x:%x:%x", uid[0], uid[1], uid[2], uid[3]);
    log_d("posting %s to %s", request_body_buffer, url);
    client.post(url, "text/plain", request_body_buffer);
    client.endRequest();
    return client.responseStatusCode();
}

void process_network()
{
    network_timer.tick();
}

char* get_network_status_string() {
  switch (network_status) {
    case NET_STATUS_INVALID_SSID:
      return "INVALID";
    case NET_STATUS_DISCONNECTED:
      return "DISCONNECTED";
    case NET_STATUS_FAILED:
      return "FAILED";
    case NET_STATUS_IDLE:
      return "IDLE";
    case NET_STATUS_CONN_LOST:
      return "LOST";
    case NET_STATUS_OK:
      return "CONNECTED";
  }
}
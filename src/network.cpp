#include "network.h"
#include "buzzer.h"
#include "status.h"
#include "storage.h"
#include "GtSound.h"

uint8_t network_status = NET_STATUS_IDLE;
uint8_t disconnected_seconds = 0;
char response_body_buffer[RES_BUFFER_SIZE] = {};
char request_body_buffer[REQ_BUFFER_SIZE] = {};

Timer<3, millis> network_timer;
uint8_t watchdog_timer = 0;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, STRINGIFY(AD_SERVER_ADDRESS), AD_SERVER_PORT);

bool really_connected = false;
char ip_address[21] = "";
char gt_ssid[GT_MEM_SIZE_SSID];
char gt_pass[GT_MEM_SIZE_PASS];
bool busy = false;
bool ever_connected = false;

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
            log_i("Connected to %s with IP address: %s", gt_ssid, ip_address);
            ever_connected = true;
            // TODO: this runs in a separate task and requires SPI communication
            // which is being used by RFID reader and display
            // gt_sound.connectedToWiFi();
        }
    }
    if (!is_connected()) {
        disconnected_seconds++;
        really_connected = false;
    }
    if (!is_connected() && ever_connected) {
        gt_sound.disconnectedFromServer();
    }
    if (disconnected_seconds > 10) {
        ad_buzzer.beep(5);
        Serial.println("trying to reconnect...");
        disconnected_seconds = 0;
        gt_connect();
    }
    process_network_async();
    return true;
}

void initialize_network() {
    gt_connect();
    network_timer.every(1000, network_status_callback);
}

void gt_connect() {
  ad_buzzer.beep(1);
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
    return response_body_buffer[0];
}

int post_to_endpoint(const char* url, byte uid[]) {
    busy = true;
    client.beginRequest();
    client.setHttpResponseTimeout(8000);
    memset(request_body_buffer, 0, REQ_BUFFER_SIZE);
    sprintf(request_body_buffer, "%x:%x:%x:%x", uid[0], uid[1], uid[2], uid[3]);
    log_d("posting %s to %s", request_body_buffer, url);
    client.post(url, "text/plain", request_body_buffer);
    client.endRequest();
    int responseCode = client.responseStatusCode();
    memset(response_body_buffer, 0, RES_BUFFER_SIZE);
    strcpy(response_body_buffer, client.responseBody().c_str());
    busy = false;
    return responseCode;
}

void process_network()
{
    network_timer.tick();
}

const char* get_network_status_string() {
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
    default:
      return "N/A";
  }
}

void process_network_async() {
    if (watchdog_timer < 3) {
        watchdog_timer++;
        return;
    }
    watchdog_timer = 0;
    if (is_connected()) {
        HttpClient c = HttpClient(wifi, STRINGIFY(AD_SERVER_ADDRESS), AD_SERVER_PORT);
        c.setHttpResponseTimeout(2000);
        c.beginRequest();
        int res = c.get(STRINGIFY(URL_HEALTH));
        c.endRequest();
        int code = c.responseStatusCode();
        // log_d("connection watchdog: %d res: %d", code, res);
        really_connected = (res == 0) && (code == 200);
        c.stop();
    }
}
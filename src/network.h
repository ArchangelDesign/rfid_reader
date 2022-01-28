#include <WiFi.h>
// #include <HTTPClient.h>
#include <ArduinoHttpClient.h>
#include <arduino-timer.h>

const char* ssid = "";
const char* password = "";
const char* server_address = "192.168.0.190";
const int server_port = 80;

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

uint8_t network_status = NET_STATUS_IDLE;
uint8_t disconnected_seconds = 0;
char response_body_buffer[RES_BUFFER_SIZE] = {};
char request_body_buffer[REQ_BUFFER_SIZE] = {};

Timer<3, millis> network_timer;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server_address, server_port);
int8_t really_connected = 1;
char ip_address[21] = "";

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
        WiFi.begin(ssid, password);
    }
    if (is_connected()) {
        really_connected = 1;
        client.setTimeout(1000);
        client.beginRequest();
        int res = client.get("/tap-in.php");
        client.endRequest();
        really_connected = res;
    }
    return true;
}

void initialize_network() {
    buzzer_beep(1);
    WiFi.begin(ssid, password);
    network_timer.every(1000, network_status_callback);
}

uint8_t tap_in(byte uid[]) {
  
    if (!is_connected()) {
        log_e("cannot verify UID without network connection.");
        buzzer_long_beeps(10);
        return ERR_NO_CONNECTION;
    }
    client.beginRequest();
    client.setTimeout(5000);
    memset(request_body_buffer, 0, REQ_BUFFER_SIZE);
    sprintf(request_body_buffer, "%x:%x:%x:%x", uid[0], uid[1], uid[2], uid[3]);

    log_d("posting: %s", request_body_buffer);
    client.post("/tap-in.php", "text/plain", request_body_buffer);
    
    client.endRequest();
    int response_code = client.responseStatusCode();
    log_d("response code: %d", response_code);
    if (response_code != 200) {
        return ERR_SERVER_ERROR;
    }
    // client.responseBody().getBytes(response_body_buffer, client.responseBody().length());
    memset(response_body_buffer, 0, RES_BUFFER_SIZE);
    strcpy(response_body_buffer, client.responseBody().c_str());
    for (int i = 0; i < RES_BUFFER_SIZE; i++) {
        Serial.print(response_body_buffer[i]);
    }
    Serial.println();
    uint8_t first_byte = response_body_buffer[0];
    Serial.println(first_byte);
    return first_byte;
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
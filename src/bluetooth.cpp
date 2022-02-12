#include "bluetooth.h"
#include "status.h"
#include "storage.h"
#include "buzzer.h"



BluetoothSerial SerialBT;
byte single_byte_buffer;
char bt_buffer[BT_BUFFER_SIZE] = {};
bt_state_t bt_current_state = gt_state_normal;

void initialize_blueetooth() {
    log_d("starting BT serial...");
    SerialBT.begin(STRINGIFY(BT_DEVICE_NAME));
    SerialBT.onAuthComplete(bt_auth_completed_callback);
    SerialBT.onData(bt_data_received);
    SerialBT.onConfirmRequest(bt_request_confirmed);
    SerialBT.register_callback(bt_callback);
    esp_bt_gap_register_callback(bt_gap_callback);
    ad_buzzer.beep(1);
}

void bt_process() {

}

void bt_auth_completed_callback(boolean success) {

}

void bt_request_confirmed(uint32_t num_val) {
    log_d("request %d", num_val);
}

void bt_callback (esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    log_d("BT Event: %d", event);
    if (event == ESP_SPP_SRV_OPEN_EVT) {
        ad_buzzer.beep(3);
    }
    if (event == ESP_SPP_CLOSE_EVT) {
        ad_buzzer.beep(1);
    }
}

void bt_gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param) {
    log_d("BT GAP Event: %d", event);
}

void bt_data_received(const uint8_t *buffer, size_t size) {
    log_d("received %d bytes of data", size);
    if (size > BT_BUFFER_SIZE) {
        log_e("buffer is too large.");
        SerialBT.write((uint8_t*)"TOO LARGE\n", 10);
        return;
    }
    memset(bt_buffer, 0, BT_BUFFER_SIZE);
    int index = 0;
    for (int i = 0; i < size; i++) {
        if (buffer[i] < 40) {
            continue;
        }
        bt_buffer[index] = buffer[i];
        index++;
    }
    gt_bt_command_received();
}

void bt_send(const char *buf) {
    SerialBT.write((uint8_t*)buf, strlen(buf));
}

void gt_bt_command_received() {
    log_d("data: %s", bt_buffer);
    if (bt_current_state == gt_state_ssid) {
        SerialBT.write((uint8_t*)"SET\n", 4);
        bt_current_state = gt_state_normal;
        gt_mem_set_ssid(bt_buffer);
        ad_buzzer.beep(3);
        return;
    }
    if (bt_current_state == gt_state_password) {
        SerialBT.write((uint8_t*)"SET\n", 4);
        bt_current_state = gt_state_normal;
        gt_mem_set_pass(bt_buffer);
        ad_buzzer.beep(3);
        return;
    }
    if (strcmp(bt_buffer, "ssid") == 0) {
        bt_current_state = gt_state_ssid;
        SerialBT.write((uint8_t*)"NEW SSID: ", 10);
        ad_buzzer.beep(1);
        return;
    }
    if (strcmp(bt_buffer, "password") == 0) {
        bt_current_state = gt_state_password;
        SerialBT.write((uint8_t*)"NEW PASSWORD: ", 14);
        ad_buzzer.beep(1);
        return;
    }
    if (strcmp(bt_buffer, "tapin") == 0) {
        set_mode_tap_in();
        ad_buzzer.beep(2);
    }
    if (strcmp(bt_buffer, "tapout") == 0) {
        set_mode_tap_out();
        ad_buzzer.beep(2);
    }
    if (strcmp(bt_buffer, "reboot") == 0) {
        ESP.restart();
        return;
    }
    SerialBT.write((uint8_t*)"ACK", 3);
}
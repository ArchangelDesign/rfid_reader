#include "bluetooth.h"
#include "status.h"

BluetoothSerial SerialBT;
byte single_byte_buffer;
char bt_buffer[BT_BUFFER_SIZE] = {};

void initialize_blueetooth() {
    log_d("staring BT serial...");
    SerialBT.begin("GT RFID Scaner");
    SerialBT.onAuthComplete(bt_auth_completed_callback);
    SerialBT.onData(bt_data_received);
}

void bt_process() {

}

void bt_auth_completed_callback(boolean success) {

}

void bt_data_received(const uint8_t *buffer, size_t size) {
    log_d("received %d bytes of data", size);
    if (size > BT_BUFFER_SIZE) {
        log_e("buffer is too large.");
        return;
    }
    memset(bt_buffer, NULL, BT_BUFFER_SIZE);
    int index = 0;
    for (int i = 0; i < size; i++) {
        if (buffer[i] < 40) {
            continue;
        }
        bt_buffer[index] = buffer[i];
        index++;
    }

    log_d("data: %s", bt_buffer);
    if (strcmp(bt_buffer, "tapin") == 0) {
        set_mode_tap_in();
    }
    if (strcmp(bt_buffer, "tapout") == 0) {
        set_mode_tap_out();
    }
    SerialBT.write((uint8_t*)"ACK", 3);
}

void bt_send(char *buf) {
    SerialBT.write((uint8_t*)buf, strlen(buf));
}
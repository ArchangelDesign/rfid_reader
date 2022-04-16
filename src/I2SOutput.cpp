
#include <Arduino.h>
#include "driver/i2s.h"
#include <math.h>
#include <SPIFFS.h>
#include <FS.h>

#include "SampleSource.h"
#include "I2SOutput.h"

#define NUM_FRAMES_TO_SEND 256

void i2sWriterTask(void *param)
{
    I2SOutput *output = (I2SOutput *)param;
    int availableBytes = 0;
    int buffer_position = 0;
    output->m_busy = true;
    Frame_t frames[NUM_FRAMES_TO_SEND];
    bool has_more_data = output->m_sample_generator->hasMoreData();
    digitalWrite(2, HIGH);
    while (has_more_data)
    {
        // wait for some data to be requested
        i2s_event_t evt;
        if (xQueueReceive(output->m_i2sQueue, &evt, portMAX_DELAY) == pdPASS)
        {
            if (evt.type == I2S_EVENT_TX_DONE)
            {
                size_t bytesWritten = 0;
                do
                {
                    if (availableBytes == 0)
                    {
                        availableBytes = output->m_sample_generator->getFrames(frames, NUM_FRAMES_TO_SEND);
                        availableBytes *= sizeof(Frame_t);
                        buffer_position = 0;
                    }
                    // do we have something to write?
                    if (availableBytes > 0)
                    {
                        i2s_write(I2S_NUM_0, buffer_position + (uint8_t *)frames,
                                  availableBytes, &bytesWritten, portMAX_DELAY);
                        availableBytes -= bytesWritten;
                        buffer_position += bytesWritten;
                    }
                    has_more_data = output->m_sample_generator->hasMoreData();
                } while (has_more_data);
            }
        }
    }
    log_d("playback done.");
    digitalWrite(2, LOW);
    delay(100);
    delete output->m_sample_generator;
    Serial.print("Max Stack size: ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
    output->m_busy = false;
    vTaskDelete(output->m_i2sWriterTaskHandle);
}

i2s_config_t I2SOutput::get_config()
{
    return {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = AD_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 256
    };
}

// void I2SOutput::startOrSkip(SampleSource *sample_generator)
// {
//     if (isBusy()) {
//         return;
//     }
//     m_sample_generator = sample_generator;
//     TaskHandle_t writerTaskHandle;
//     // xTaskCreate(i2sWriterTask, "i2s Writer Task", 4096, this, 0, &writerTaskHandle);
//     xTaskCreatePinnedToCore(i2sWriterTask, "i2s Writer Task", 3096, this, 0, &writerTaskHandle, 1);
// }

void I2SOutput::startOrWait(SampleSource *sample_generator)
{
    #ifdef AD_DISABLE_SOUND
    return;
    #endif
    if (isBusy()) {
        while (isBusy()) {
            delay(500);
        }
    }
    
    delay(100);
    m_sample_generator = sample_generator;
    xTaskCreate(i2sWriterTask, "i2s Writer Task", 3096, this, 1, &m_i2sWriterTaskHandle);
    // xTaskCreatePinnedToCore(i2sWriterTask, "i2s Writer Task", 4096, this, 1, &m_i2sWriterTaskHandle, 0);
}

void I2SOutput::initialize()
{
    i2s_config_t i2sConfig = get_config();
    i2s_driver_install(I2S_NUM_0, &i2sConfig, 4, &m_i2sQueue);
    static const i2s_pin_config_t pin_config = {
        .bck_io_num = AD_I2S_BCLK,
        .ws_io_num = AD_I2S_LRC,
        .data_out_num = AD_I2S_DIN,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void I2SOutput::shutdown()
{
    i2s_driver_uninstall(I2S_NUM_0);
}
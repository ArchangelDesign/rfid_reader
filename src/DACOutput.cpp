
#include <Arduino.h>
#include "driver/i2s.h"
#include <math.h>
#include <SPIFFS.h>
#include <FS.h>

#include "SampleSource.h"
#include "DACOutput.h"

#define NUM_FRAMES_TO_SEND 256

void dacWriterTask(void *param)
{
    DACOutput *output = (DACOutput *)param;
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
    output->m_busy = false;
    vTaskDelete(output->m_i2sWriterTaskHandle);
}

i2s_config_t DACOutput::get_config()
{
    return {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
        .sample_rate = AD_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL3,
        .dma_buf_count = 2,
        .dma_buf_len = 256
    };
}

void DACOutput::startOrWait(SampleSource *sample_generator)
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
    xTaskCreate(dacWriterTask, "DAC Writer Task", 4096, this, 1, &m_i2sWriterTaskHandle);
    // xTaskCreatePinnedToCore(i2sWriterTask, "i2s Writer Task", 4096, this, 1, &m_i2sWriterTaskHandle, 0);
}

void DACOutput::initialize()
{
    i2s_config_t i2sConfig = get_config();
    i2s_driver_install(I2S_NUM_0, &i2sConfig, 4, &m_i2sQueue);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
    i2s_zero_dma_buffer(I2S_NUM_0);
}

void DACOutput::shutdown()
{
    i2s_driver_uninstall(I2S_NUM_0);
}
#ifndef __sampler_base_h__
#define __sampler_base_h__

#include <Arduino.h>
#include "driver/i2s.h"

class SampleSource;

/**
 * Base Class for both the ADC and I2S sampler
 **/
class DACOutput
{
private:
    TaskHandle_t m_i2sWriterTaskHandle;
    QueueHandle_t m_i2sQueue;
    SampleSource *m_sample_generator;
    bool m_busy = false;
    i2s_config_t get_config();

public:
    void startOrSkip(SampleSource *sample_generator);
    void startOrWait(SampleSource *sample_generator);
    friend void i2sWriterTask(void *param);
    bool isBusy() { return m_busy; }
    void initialize();
};

#endif
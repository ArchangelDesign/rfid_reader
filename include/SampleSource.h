#ifndef __sample_source_h__
#define __sample_source_h__

#include <Arduino.h>

typedef struct
{
    uint16_t left;
} Frame_t;

/**
 * Base class for our sample generators
 **/
class SampleSource
{
public:
    virtual int sampleRate() = 0;
    // This should fill the samples buffer with the specified number of frames
    virtual void getFrames(Frame_t *frames, int number_frames) = 0;

    virtual bool hasMoreData();
};

#endif
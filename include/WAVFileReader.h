#ifndef __wav_file_reader_h__
#define __wav_file_reader_h__

#include <SPIFFS.h>
#include <FS.h>
#include "SampleSource.h"

class WAVFileReader : public SampleSource
{
private:
    int m_num_channels;
    int m_sample_rate;
    File m_file;
    bool m_is_open = false;
public:
    WAVFileReader(const char *file_name);
    virtual ~WAVFileReader();
    int sampleRate() { return m_sample_rate; }
    size_t getFrames(Frame_t *frames, int number_frames);
    bool hasMoreData();
};

#endif
#include <SD.h>
#include <FS.h>
#include "WAVFileReader.h"
#include "GtStorage.h"

#pragma pack(push, 1)
typedef struct
{
    // RIFF Header
    char riff_header[4]; // Contains "RIFF"
    int wav_size;        // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    char wave_header[4]; // Contains "WAVE"

    // Format Header
    char fmt_header[4]; // Contains "fmt " (includes trailing space)
    int fmt_chunk_size; // Should be 16 for PCM
    short audio_format; // Should be 1 for PCM. 3 for IEEE Float
    short num_channels;
    int sample_rate;
    int byte_rate;          // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    short sample_alignment; // num_channels * Bytes Per Sample
    short bit_depth;        // Number of bits per sample

    // Data
    char data_header[4]; // Contains "data"
    int data_bytes;      // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header_t;
#pragma pack(pop)

WAVFileReader::WAVFileReader(const char *file_name, GtStorage *storage)
{
    if (!storage->exists(file_name))
    {
        Serial.println("****** Failed to open file! Have you uploaed the file system?");
        return;
    }
    m_file = storage->openForReading(file_name);
    // read the WAV header
    wav_header_t wav_header;
    m_file.read((uint8_t *)&wav_header, sizeof(wav_header_t));
    // sanity check the bit depth
    if (wav_header.bit_depth != 16)
    {
        Serial.printf("ERROR: bit depth %d is not supported\n", wav_header.bit_depth);
        throw std::invalid_argument("Invalid bit depth.");
    }
    if (wav_header.sample_rate != AD_SAMPLE_RATE)
    {
        throw std::invalid_argument("Invalid sample rate.");
    }

    // Serial.printf("fmt_chunk_size=%d, audio_format=%d, num_channels=%d, sample_rate=%d, sample_alignment=%d, bit_depth=%d, data_bytes=%d\n",
    //               wav_header.fmt_chunk_size, wav_header.audio_format, wav_header.num_channels, wav_header.sample_rate, wav_header.sample_alignment, wav_header.bit_depth, wav_header.data_bytes);

    m_num_channels = wav_header.num_channels;
    m_sample_rate = wav_header.sample_rate;
    m_is_open = true;
}

WAVFileReader::~WAVFileReader()
{
    if (m_is_open) {
        m_file.close();
    }
}

size_t WAVFileReader::getFrames(Frame_t *frames, int number_frames)
{
    memset(frames, 0, number_frames);
    size_t frames_read = 0;
    if (!m_is_open) {
        return 0;
    }
    for (int i = 0; i < number_frames; i++)
    {
        if (m_file.available() == 0)
        {
            m_file.close();
            m_is_open = false;
            return frames_read;
        }
        int16_t left;
        int16_t right;
        m_file.read((uint8_t *)(&left), sizeof(int16_t));
        if (m_num_channels == 1)
        {
            right = left;
        }
        else
        {
            m_file.read((uint8_t *)(&right), sizeof(int16_t));
        }
        frames[i].left = left;// + 32768;
        // using PCM 16 bit mono
        // frames[i].right = right + 32768;
        frames_read++;
    }

    return frames_read;
}

bool WAVFileReader::hasMoreData()
{
    if (!m_is_open) {
        return false;
    }
    return m_file.available() > 0;
}
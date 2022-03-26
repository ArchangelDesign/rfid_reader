#include "GtSound.h"
#include "WAVFileReader.h"

GtSound::GtSound() {
    output = new DACOutput();
}

void GtSound::systemStarting()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl1.wav");
    output->startOrWait(sampleSource);
}

void GtSound::networkReady()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl2.wav");
    output->startOrWait(sampleSource);
}

void GtSound::bluetoothReady()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl3.wav");
    output->startOrWait(sampleSource);
}

void GtSound::systemReady()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl4.wav");
    output->startOrWait(sampleSource);
}
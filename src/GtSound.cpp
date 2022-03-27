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

void GtSound::initialize()
{
    output->initialize();
}

void GtSound::readerInitialized()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl5.wav");
    output->startOrWait(sampleSource);
}

void GtSound::eepromInitialized()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl6.wav");
    output->startOrWait(sampleSource);
}

void GtSound::connectedToWiFi()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl7.wav");
    output->startOrWait(sampleSource);
}

void GtSound::disconnectedFromServer()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl8.wav");
    output->startOrWait(sampleSource);
}

void GtSound::connecting()
{
    WAVFileReader *sampleSource = new WAVFileReader("/smpl9.wav");
    output->startOrWait(sampleSource);
}
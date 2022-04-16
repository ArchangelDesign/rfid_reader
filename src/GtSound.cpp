#include "GtSound.h"

GtSound::GtSound() {
    output = new I2SOutput();
}

WAVFileReader* GtSound::loadSound(const char* fname)
{
    #ifdef AD_DISABLE_SOUND
    return NULL;
    #endif
    return new WAVFileReader(fname);
}

void GtSound::systemStarting()
{
    output->startOrWait(loadSound("/smpl1.wav"));
}

void GtSound::networkReady()
{
    output->startOrWait(loadSound("/smpl2.wav"));
}

void GtSound::bluetoothReady()
{
    output->startOrWait(loadSound("/smpl3.wav"));
}

void GtSound::systemReady()
{
    output->startOrWait(loadSound("/smpl4.wav"));
}

void GtSound::initialize()
{
    #ifndef AD_DISABLE_SOUND
    output->initialize();
    #endif
}

void GtSound::shutdown()
{
    #ifndef AD_DISABLE_SOUND
    output->shutdown();
    #endif
}

void GtSound::readerInitialized()
{
    output->startOrWait(loadSound("/smpl5.wav"));
}

void GtSound::eepromInitialized()
{
    output->startOrWait(loadSound("/smpl6.wav"));
}

void GtSound::connectedToWiFi()
{
    output->startOrWait(loadSound("/smpl7.wav"));
}

void GtSound::disconnectedFromServer()
{
    output->startOrWait(loadSound("/smpl8.wav"));
}

void GtSound::connecting()
{
    output->startOrWait(loadSound("/smpl9.wav"));
}

void GtSound::cardDetected()
{
    output->startOrWait(loadSound("/smpl10.wav"));
}

void GtSound::invalidCard()
{
    output->startOrWait(loadSound("/smpl11.wav"));
}

void GtSound::ok()
{
    output->startOrWait(loadSound("/smpl12.wav"));
}

void GtSound::networkError()
{
    output->startOrWait(loadSound("/smpl13.wav"));
}
#include "audiostream.h"

AudioStreamBase::AudioStreamBase(unsigned int samplerate, unsigned int channels, PaSampleFormat sampleformat, unsigned int buffersize) :
    _samplerate(samplerate),
    _channels(channels),
    _sampleformat(sampleformat),
    _buffersize(buffersize)
{
    _sampleratef = (float)_samplerate;
    _samplerated = (double)_samplerate;
}

unsigned int AudioStreamBase::getSampleRate() const
{
    return _samplerate;
}

unsigned int AudioStreamBase::getChannelAmount() const
{
    return _channels;
}

PaSampleFormat AudioStreamBase::getSampleFormat() const
{
    return _sampleformat;
}

float AudioStreamBase::getSampleRateFloat() const
{
    return _sampleratef;
}

double AudioStreamBase::getSampleRateDouble() const
{
    return _samplerated;
}

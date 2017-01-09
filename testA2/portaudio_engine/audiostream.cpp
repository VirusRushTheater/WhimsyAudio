#include "audiostream.h"

AudioStream::AudioStream(unsigned int samplerate, unsigned int channels, PaSampleFormat sampleformat, unsigned int buffersize) :
    _samplerate(samplerate),
    _channels(channels),
    _sampleformat(sampleformat),
    _buffersize(buffersize)
{
}

unsigned int AudioStream::getSampleRate() const
{
    return _samplerate;
}

unsigned int AudioStream::getChannelAmount() const
{
    return _channels;
}

PaSampleFormat AudioStream::getSampleFormat() const
{
    return _sampleformat;
}

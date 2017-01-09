#pragma once

#include "portaudio.h"

class ScopedPAContext;

class AudioStream
{
    friend class ScopedPAContext;

protected:
    unsigned int        _samplerate, _channels;
    PaSampleFormat      _sampleformat;
    unsigned int        _buffersize;

    float               _sampleratef;
    double              _samplerated;

public:
    AudioStream(unsigned int samplerate = 44100,
                unsigned int channels = 2,
                PaSampleFormat sampleformat = paFloat32,
                unsigned int buffersize = 64);

    unsigned int    getSampleRate() const;
    float           getSampleRateFloat() const;
    double          getSampleRateDouble() const;

    unsigned int getChannelAmount() const;
    PaSampleFormat getSampleFormat() const;

    virtual int audioOut(void *outputBuffer, unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags) = 0;
};

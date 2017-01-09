#pragma once

#include "portaudio.h"
#include "../whimsycore.h"
#include "audiostream.h"

class ScopedPAContext
{
private:
    PaDeviceIndex       _outputdev;
    PaStreamParameters  _strpars;
    AudioStream*        _as;
    PaStream*           _stream;

    bool                _isplaying;

    PaError             _result;

    static int apiCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

public:
    ScopedPAContext();
    ~ScopedPAContext();

    void    setStream(AudioStream& as);
    bool    startStream(unsigned int timeout_ms = 0);
    bool    stopStream();
    bool    closeStream();

    PaError result() const;

};

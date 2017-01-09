#pragma once

#include "audiostream.h"

class MetronomeTest : public AudioStream
{
private:
    unsigned long long int t1, t2, t;
    double bpm;
    float volume;

public:
    MetronomeTest() : t1(0), t2(0), t(0), bpm(120.0), volume(0.5f)
    {}

    int audioOut(void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);
};

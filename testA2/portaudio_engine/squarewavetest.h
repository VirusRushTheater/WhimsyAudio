#pragma once

#include "audiostream.h"

class SquareWaveTest : public AudioStream
{
private:
    unsigned long long int t;
    float amplitude, frequency, dutycycle;

public:
    SquareWaveTest() : t(0){}
    int audioOut(void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags);

    void setAmplitude(float _amplitude){amplitude = _amplitude;}
    void setFrequency(float _frequency){frequency = _frequency;}
    void setDutyCycle(float _dutycycle){dutycycle = _dutycycle;}
};

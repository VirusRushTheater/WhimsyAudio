#pragma once

#include "audiostream.h"

class SquareWaveTest : public AudioStream<char, 11050, 2>
{
private:
    unsigned long int t;
    unsigned long int period;

public:
    SquareWaveTest(float freq) :
        t(0), period(11050/freq)
    {}

    int stereoOut(StereoSample<char> *samples, unsigned long length)
    {
        for(unsigned int i = 0; i < length; i++,
            t++)
        {
            if(t < period/2)
                samples[i].set(40);
            else if(t < period)
                samples[i].set(-40);
            else
            {
                t = 0;
                samples[i].set(40);
            }
        }
        return 0;
    }
};

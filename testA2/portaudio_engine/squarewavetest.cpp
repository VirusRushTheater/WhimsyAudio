#include "squarewavetest.h"

struct StereoAudioOut
{
    float left, right;
};

int SquareWaveTest::audioOut(void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    float* mono_out =               (float*) outputBuffer;
    StereoAudioOut* stereo_out =    (StereoAudioOut*) outputBuffer;

    float period =                  ((float)getSampleRate()) / frequency;
    float ontime =                  period * dutycycle;

    unsigned long i;

    // Mono
    if(getChannelAmount() == 1)
    {
        for(i = 0; i < framesPerBuffer; i++, t++)
        {
            if(t < ontime)
                mono_out[i] = amplitude;
            else if(t < period)
                mono_out[i] = -1.0f * amplitude;
            else
            {
                t = 1;
                mono_out[i] = amplitude;
            }
        }
    }

    // Stereo
    else if(getChannelAmount() == 2)
    {
        for(i = 0; i < framesPerBuffer; i++, t++)
        {
            if(t < ontime)
            {
                stereo_out[i].left =    amplitude;
                stereo_out[i].right =   amplitude;
            }
            else if(t < period)
            {
                stereo_out[i].left =    -1.0f * amplitude;
                stereo_out[i].right =   -1.0f * amplitude;
            }
            else
            {
                t = 1;
                stereo_out[i].left =    amplitude;
                stereo_out[i].right =   amplitude;
            }
        }
    }
    // I don't know what to do with this many channels!
    else
        return paAbort;

    return 0;
}

#include "metronometest.h"

struct StereoAudioOut
{
    float left, right;
};

int MetronomeTest::audioOut(void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags)
{
    float* mono_out =               (float*) outputBuffer;
    StereoAudioOut* stereo_out =    (StereoAudioOut*) outputBuffer;

    // Accent is a 20ms square wave of 500Hz. Others are same length but 250Hz.
    const float halfperiod =        getSampleRateFloat() / (500.0f * 2.0f);
    const float duration =          getSampleRateFloat() * 0.02f;
    const float intertone_lapse =   getSampleRateFloat() * 60.0f / bpm;

    unsigned long i;

    // Mono
    if(getChannelAmount() == 1)
    {
        for(i = 0; i < framesPerBuffer; i++,
            t1++, t2++, t++)
        {
            if(t < duration)
            {
                if(t1 < halfperiod)
                {
                    mono_out[i] = volume;
                }
                else if(t1 < 2*halfperiod)
                {
                    mono_out[i] = -1.0f * volume;
                }
                else
                {
                    mono_out[i] = volume;
                    t1 = 0;
                }
            }
            else
            {
                mono_out[i] = 0.0f;
            }


            // Metronome rhythm
            if(t >= intertone_lapse)
                t = 0;
        }
    }

    // Stereo
    else if(getChannelAmount() == 2)
    {
        for(i = 0; i < framesPerBuffer; i++,
            t1++, t2++, t++)
        {
            if(t < duration)
            {
                if(t1 < halfperiod)
                {
                    stereo_out[i].left = volume;
                    stereo_out[i].right = volume;
                }
                else if(t1 < 2*halfperiod)
                {
                    stereo_out[i].left = -1.0f * volume;
                    stereo_out[i].right = -1.0f * volume;
                }
                else
                {
                    stereo_out[i].left = volume;
                    stereo_out[i].right = volume;
                    t1 = 0;
                }
            }
            else
            {
                stereo_out[i].left = 0.0f;
                stereo_out[i].right = 0.0f;
            }


            // Metronome rhythm
            if(t >= intertone_lapse)
                t = 0;
        }
    }
    // I don't know what to do with this many channels!
    else
        return paAbort;

    return 0;

}

#include "scopedPAContext.h"

AudioStream::AudioStream()
{
    setDefaultConfig();
}

AudioStream::AudioStream(unsigned int samplerate, unsigned int channels, PaSampleFormat sampleformat, unsigned int buffersize) :
    _samplerate(samplerate),
    _channelcount(channels),
    _sampleformat(sampleformat),
    _buffersize(buffersize)
{
}

bool AudioStream::setDefaultConfig()
{
    // Stereo, Float32 samples, with minimum latency.
    _channelcount =                     2;
    _sampleformat =                     paFloat32;

    // Default sample rate and buffer size.
    _samplerate =                       44100;
    _buffersize =                       64;
}

// *******************************

ScopedPAContext::ScopedPAContext() :
    _result(Pa_Initialize())
{
    // If initialization failed, throws an exception.
    if(_result != paNoError)
        throw whimsycore::Exception(NULL, whimsycore::Exception::PortAudioInitError, Pa_GetErrorText(_result));

    // Use the default output. Most cases it's the one the user wants to use. Otherwise they can use their
    // OS to change it.
    _defaultoutput =                    Pa_GetDefaultOutputDevice();
    if (_defaultoutput == paNoDevice) {
        throw whimsycore::Exception(NULL, whimsycore::Exception::PortAudioNoDevices, "No default output device.");
    }

    _strparams.suggestedLatency =       Pa_GetDeviceInfo(_defaultoutput)->defaultLowOutputLatency;
    _strparams.hostApiSpecificStreamInfo =  NULL;
}

ScopedPAContext::~ScopedPAContext()
{
    if(_result == paNoError)
    {
        Pa_Terminate();
    }
}

void ScopedPAContext::setStream(AudioStream &as)
{
    _strparams.channelCount =   as._channelcount;
    _strparams.sampleFormat =   as._sampleformat;

    _currentstream = &as;
}

PaError ScopedPAContext::result() const
{
    return _result;
}

int ScopedPAContext::audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    (void) inputBuffer;
    return dynamic_cast<AudioStream*>(userData)->audioCallback(outputBuffer, framesPerBuffer, timeInfo, statusFlags);
}

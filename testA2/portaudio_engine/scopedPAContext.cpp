#include "scopedPAContext.h"


bool AudioStream::setDefaultConfig()
{
    _pars.device =                      _defaultoutput;

    // Stereo, Float32 samples, with minimum latency.
    _pars.channelCount =                2;
    _pars.sampleFormat =                paFloat32;
    _pars.suggestedLatency =            Pa_GetDeviceInfo(_defaultoutput)->defaultLowOutputLatency;
    _pars.hostApiSpecificStreamInfo =   NULL;

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
        return false;
    }
}

ScopedPAContext::~ScopedPAContext()
{
    if(_result == paNoError)
    {
        Pa_Terminate();
    }
}

PaError ScopedPAContext::result() const
{
    return _result;
}

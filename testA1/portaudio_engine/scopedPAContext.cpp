#include "scopedPAContext.h"

ScopedPAContext::ScopedPAContext() :
    _result(Pa_Initialize())
{
    if(_result != paNoError)
        throw whimsycore::Exception(NULL, whimsycore::Exception::PortAudioInitError, Pa_GetErrorText(_result));
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

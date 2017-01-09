#include "scopedPAContext.h"

ScopedPAContext::ScopedPAContext() :
    _result(Pa_Initialize())
{
    // Inits PortAudio
    if(_result != paNoError)
        throw whimsycore::Exception(NULL, whimsycore::Exception::PortAudioInitError, Pa_GetErrorText(_result));

    // Opens the default output device. If fails, throws an exception.
    _outputdev =    Pa_GetDefaultOutputDevice();
    if(_outputdev == paNoDevice)
        throw whimsycore::Exception(NULL, whimsycore::Exception::PortAudioNoDevices, "No output devices found.");

    // Some stream configurations, such as output device and low output latency.
    memset(&_strpars, 0, sizeof(PaStreamParameters));
    _strpars.device =                       _outputdev;
    _strpars.hostApiSpecificStreamInfo =    NULL;
    _strpars.suggestedLatency =             Pa_GetDeviceInfo(_outputdev)->defaultLowOutputLatency;

    _isplaying =                            false;
    _stream =                               NULL;
}

ScopedPAContext::~ScopedPAContext()
{
    // Terminates PortAudio
    if(_result == paNoError)
    {
        Pa_Terminate();
    }
}

PaError ScopedPAContext::result() const
{
    return _result;
}

void ScopedPAContext::setStream(AudioStream &as)
{
    _as = &(as);

    _strpars.channelCount =     as._channels;
    _strpars.sampleFormat =     as._sampleformat;

    if(_stream != NULL)
    {
        stopStream();
        closeStream();
    }

    Pa_OpenStream(&_stream, NULL, &_strpars, (double)as._samplerate, paFramesPerBufferUnspecified,
                  paClipOff, ScopedPAContext::apiCallback, this->_as);
}

bool ScopedPAContext::startStream(unsigned int timeout_ms)
{
    if (_stream == NULL)
        return false;

    PaError err =   Pa_StartStream(_stream);
    if(timeout_ms != 0)
        Pa_Sleep(timeout_ms);

    return (err == paNoError);
}

bool ScopedPAContext::stopStream()
{
    if (_stream == NULL)
        return false;

    PaError err =   Pa_StopStream( _stream );

    return (err == paNoError);
}

bool ScopedPAContext::closeStream()
{
    if (_stream == NULL)
        return false;

    PaError err =   Pa_CloseStream(_stream);
    _stream =       NULL;

    return (err == paNoError);
}

int ScopedPAContext::apiCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    AudioStream* current_stream = static_cast<AudioStream*>(userData);
    (void) inputBuffer;

    // Cast this ScopedPAContext._as as the userData used here.
    return(current_stream->audioOut(outputBuffer, framesPerBuffer, timeInfo, statusFlags));
}

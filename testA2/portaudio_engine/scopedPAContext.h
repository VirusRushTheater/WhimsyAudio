#pragma once

#include "portaudio.h"
#include "../whimsycore.h"

class ScopedPAContext;

class AudioStream
{
    friend class ScopedPAContext;

private:
    unsigned int        _samplerate, _buffersize, _channelcount;
    PaSampleFormat      _sampleformat;

    bool                _isplaying;

public:
    AudioStream();
    AudioStream(unsigned int samplerate = 44100,
                unsigned int channels = 2,
                PaSampleFormat sampleformat = paFloat32,
                unsigned int buffersize = 64);

    bool        setDefaultConfig();
    virtual int audioCallback(void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags) = 0;
};

/**
 * @brief This class is in charge to initialize and terminate PortAudio contexts. It's in charge to manage audio output
 * hardware and to start and stop different streams.
 */
class ScopedPAContext
{
private:
    PaDeviceIndex       _defaultoutput;
    PaError             _result;
    PaStreamParameters  _strparams;

    AudioStream*        _currentstream;

    static int          audioCallback(const void *inputBuffer, void *outputBuffer,
                                           unsigned long framesPerBuffer,
                                           const PaStreamCallbackTimeInfo* timeInfo,
                                           PaStreamCallbackFlags statusFlags,
                                           void *userData);

public:
    /**
     * @brief Constructor. Initializes PortAudio and gets details from the default Output Device.
     */
    ScopedPAContext();

    /**
     * @brief Destructor. Deinitializes PortAudio.
     */
    ~ScopedPAContext();

    /**
     * @brief Sets a stream to play. Control it with startStream() and stopStream().
     * @param as        Reference to an AudioStream (or a child object)
     */
    void    setStream(AudioStream& as);

    //void    startStream(unsigned int timeoutms = 0);
    //void    stopStream();

    /**
     * @brief Returns the last error code.
     * @return
     */
    PaError result() const;
};

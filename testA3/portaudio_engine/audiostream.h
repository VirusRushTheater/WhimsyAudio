#pragma once

#include "portaudio.h"
#include <iostream>

class ScopedPAContext;

/**
 * Merely a placeholder to allow this template to grab 24-bit samples.
 */
#ifndef int24_t
typedef long int    int24_t;
#endif

/**
 * @brief Some template black magic to convert native C++ types into PaSampleFormat.
 * I had to do some placeholder for a 24 bit format sample. Use int24_t to define this typing.
 */
template <typename T>
struct SampleFormat
{
    static constexpr PaSampleFormat format =    paFloat32;
    static constexpr T              MAXVAL =    1.0f;
    static constexpr T              ZERO =      0.0f;
    static constexpr T              MINVAL =    -1.0f;

    static constexpr unsigned char  BPS =       4;
};

template <>
struct SampleFormat <unsigned char>
{
    static constexpr PaSampleFormat format =    paUInt8;
    static constexpr unsigned char  MAXVAL =    255;
    static constexpr unsigned char  ZERO =      128;
    static constexpr unsigned char  MINVAL =    0;

    static constexpr unsigned char  BPS =       1;
};

template <>
struct SampleFormat <char>
{
    static constexpr PaSampleFormat format =    paInt8;
    static constexpr char           MAXVAL =    127;
    static constexpr char           ZERO =      0;
    static constexpr char           MINVAL =    -128;

    static constexpr unsigned char  BPS =       1;
};

template <>
struct SampleFormat <short int>
{
    static constexpr PaSampleFormat format =    paInt16;
    static constexpr short int      MAXVAL =    32767;
    static constexpr short int      ZERO =      0;
    static constexpr short int      MINVAL =    -32768;

    static constexpr unsigned char  BPS =       2;
};

template <>
struct SampleFormat <int24_t>
{
    static constexpr PaSampleFormat format =    paInt24;
    static constexpr int24_t        MAXVAL =    8388607;
    static constexpr int24_t        ZERO =      0;
    static constexpr int24_t        MINVAL =    -8388608;

    static constexpr unsigned char  BPS =       3;
};

template <>
struct SampleFormat <int>
{
    static constexpr PaSampleFormat format =    paInt32;
    static constexpr int            MAXVAL =    2147483647;
    static constexpr int            ZERO =      0;
    static constexpr int            MINVAL =    -2147483648;

    static constexpr unsigned char  BPS =       4;
};

/**
 * A structure made for easy access to stereo samples.
 */
template <typename T>
struct StereoSample
{
    T left, right;

    /**
     * @brief Sets both left and right samples with the same value.
     * @param value
     */
    inline void set(T value)
    {
        left = right = value;
    }
};

/**
 * @brief Base clase for internal use. Use AudioStream instead!
 */
class AudioStreamBase
{
    friend class ScopedPAContext;

protected:
    unsigned int        _samplerate, _channels;
    PaSampleFormat      _sampleformat;
    unsigned int        _buffersize;

    float               _sampleratef;
    double              _samplerated;

public:
    AudioStreamBase(unsigned int samplerate = 44100,
                unsigned int channels = 2,
                PaSampleFormat sampleformat = paFloat32,
                unsigned int buffersize = 64);

    unsigned int    getSampleRate() const;
    float           getSampleRateFloat() const;
    double          getSampleRateDouble() const;

    unsigned int    getChannelAmount() const;
    PaSampleFormat  getSampleFormat() const;

    virtual int audioOut(void *outputBuffer, unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags) = 0;
};

/**
 * @brief Inheritable class to control output to speakers. Template defines the sample format (float or integer),
 * sample rate (in Hz) and amount of channels. Default is floating point samples, 44,100Hz and 2 channels (stereo)
 */
template<typename T, unsigned int samplerate = 44100, unsigned char channels = 2>
class AudioStream : public AudioStreamBase
{
public:

    AudioStream() :
        AudioStreamBase(samplerate, channels, SampleFormat<T>::format, 64)
    {
    }

    /**
     * @brief Callback method which will be called once this stream is assigned to a PAContext and requests audio data.
     * To use it, make an AudioStream subclass and reimplement audioOut if you need some low level managing. Else, you
     * might want to reimplement monoOut or stereoOut, which are much easy to handle.
     *
     * @param outputBuffer      Buffer you will have to fill.
     * @param framesPerBuffer   How many frames (samples) is your buffer.
     * @param timeInfo          Some timestamp PortAudio uses. Irrelevant in most cases.
     * @param statusFlags       Status flags PortAudio uses. Irrelevant in most cases.
     *
     * @return  Make this function return 0 if you don't want to stop the stream from here. You can return either
     * paComplete or paAbort otherwise. Consult PortAudio documentation about these.
     */
    virtual int audioOut(void *outputBuffer, unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags)
    {
        if(channels == 1)
            return monoOut((T*)outputBuffer, framesPerBuffer);
        else if(channels == 2)
            return stereoOut((StereoSample<T>*)outputBuffer, framesPerBuffer);
        else
            return paAbort;
    }

    /**
     * @brief Callback method to be reimplemented in a child function if you have a Mono stream.
     * @param samples   Sample array to be filled.
     * @param length    How many samples must be filled.
     *
     * @return  Make this function return 0 if you don't want to stop the stream from here. You can return either
     * paComplete or paAbort otherwise. Consult PortAudio documentation about these.
     */
    virtual int monoOut(T* samples, unsigned long length)
    {
        return paAbort;
    }

    /**
     * @brief Callback method to be reimplemented in a child function if you have a Stereo stream.
     * @param samples   Sample array to be filled.
     * @param length    How many samples must be filled.
     *
     * @return  Make this function return 0 if you don't want to stop the stream from here. You can return either
     * paComplete or paAbort otherwise. Consult PortAudio documentation about these.
     */
    virtual int stereoOut(StereoSample<T>* samples, unsigned long length)
    {
        return paAbort;
    }
};

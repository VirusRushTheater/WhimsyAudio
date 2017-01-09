#pragma once

#include "portaudio.h"
#include "../whimsycore.h"

class ScopedPAContext
{
private:
    PaError _result;

public:
    ScopedPAContext();
    ~ScopedPAContext();

    PaError result() const;
};

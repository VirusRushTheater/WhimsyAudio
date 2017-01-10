#include "testA3mw.h"

TestA3MW::TestA3MW(QWidget *parent)
{
    this->setWindowTitle("Metronome with PortAudio and Qt5");
}


TestA3MW::~TestA3MW()
{
}

void TestA3MW::setPortAudioContext(ScopedPAContext* pactx)
{
    _pactx = pactx;
}

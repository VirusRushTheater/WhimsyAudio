#include "testA1mw.h"

TestA1MW::TestA1MW(QWidget *parent)
{
}

TestA1MW::~TestA1MW()
{
}

void TestA1MW::setPortAudioContext(ScopedPAContext* pactx)
{
    _pactx = pactx;
}

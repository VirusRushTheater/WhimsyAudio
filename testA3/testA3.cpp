#include <QApplication>
#include <QDebug>

#include "gui/testA3mw.h"
#include "portaudio_engine/scopedPAContext.h"
#include "portaudio_engine/squarewavetest.h"
#include "whimsycore.h"

using namespace whimsycore;

int main(int argc, char** argv)
{
    /*QApplication        testa3_app(argc, argv);
    TestA3MW            testwidget;

    ScopedPAContext     pactx;

    testwidget.setPortAudioContext(&pactx);
    testwidget.show();

    return testa3_app.exec();*/

    ScopedPAContext     pactx;
    SquareWaveTest      sqw(440.0f * 1.26f);

    pactx.setStream(sqw);
    pactx.startStream(2000);

    return 0;
}

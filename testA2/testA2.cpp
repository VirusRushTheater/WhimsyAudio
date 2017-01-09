#include <QApplication>

#include "gui/testA1mw.h"
#include "portaudio_engine/scopedPAContext.h"
#include "portaudio_engine/squarewavetest.h"
#include "whimsycore.h"

using namespace whimsycore;

int main(int argc, char** argv)
{
 //   QApplication        testa1_app(argc, argv);
    ScopedPAContext     pactx;
 //   TestA1MW            testwidget;
    SquareWaveTest      sqt;

 //   testwidget.setPortAudioContext(&pactx);
    //testwidget.show();

    sqt.setAmplitude(0.4f);
    sqt.setDutyCycle(0.01f);
    sqt.setFrequency(440.0f);

    pactx.setStream(sqt);
    pactx.startStream(5000);

    return 0;
    //return testa1_app.exec();
}

#include <QApplication>

#include "gui/testA1mw.h"
#include "portaudio_engine/scopedPAContext.h"
#include "whimsycore.h"

using namespace whimsycore;

int main(int argc, char** argv)
{
    QApplication        testa1_app(argc, argv);
    ScopedPAContext     pactx;
    TestA3MW            testwidget;

    testwidget.setPortAudioContext(&pactx);
    testwidget.show();

    return testa1_app.exec();
}

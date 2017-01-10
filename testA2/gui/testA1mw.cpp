#include "testA1mw.h"

TestA1MW::TestA1MW(QWidget *parent)
{
    vl = new QGridLayout();

    sp1 = new QSpinBox();
    sl1 = new QSlider(Qt::Horizontal);
    sl2 = new QSlider(Qt::Horizontal);
    pb1 = new QPushButton("Start!");

    lb1 = new QLabel("BPM");
    lb2 = new QLabel("Volume");

    pb1->setCheckable(true);

    sp1->setMinimum(20);
    sp1->setMaximum(250);
    sp1->setValue(120);

    sl1->setMinimum(20);
    sl1->setMaximum(250);
    sl1->setValue(120);

    sl2->setMinimum(0);
    sl2->setMaximum(100);
    sl2->setValue(50);

    vl->addWidget(lb1, 0, 0);
    vl->addWidget(sp1, 0, 1);
    vl->addWidget(sl1, 0, 2);
    vl->addWidget(pb1, 0, 3);

    vl->addWidget(lb2, 1, 0);
    vl->addWidget(sl2, 1, 1, 1, 3);

    this->setLayout(vl);

    QObject::connect(pb1, SIGNAL(clicked(bool)), this, SLOT(startstopMetronome()));
    QObject::connect(sp1, SIGNAL(valueChanged(int)), this, SLOT(changeBPM(int)));
    QObject::connect(sl1, SIGNAL(sliderMoved(int)), this, SLOT(changeBPM(int)));
    QObject::connect(sl1, SIGNAL(valueChanged(int)), this, SLOT(changeBPM(int)));

    QObject::connect(sl2, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
    QObject::connect(sl2, SIGNAL(valueChanged(int)), this, SLOT(changeVolume(int)));

    this->setWindowTitle("Metronome with PortAudio and Qt5");
}

void TestA1MW::startstopMetronome()
{
    if(pb1->isChecked())
    {
        pb1->setText("Stop!");
        _pactx->startStream();
    }
    else
    {
        pb1->setText("Start!");
        _pactx->stopStream();
    }
}

void TestA1MW::changeBPM(int newbpm)
{
    sl1->setValue(newbpm);
    sp1->setValue(newbpm);

    mtest.changeBPM(newbpm);
}

void TestA1MW::changeVolume(int percentage)
{
    mtest.changeVolume((float)percentage / 100.0f );
}

TestA1MW::~TestA1MW()
{
    _pactx->stopStream();
    _pactx->closeStream();

    delete(vl);
    delete(sl1);
    delete(sl2);
    delete(pb1);
    delete(lb1);
    delete(lb2);
}

void TestA1MW::setPortAudioContext(ScopedPAContext* pactx)
{
    _pactx = pactx;
    _pactx->setStream(mtest);
}

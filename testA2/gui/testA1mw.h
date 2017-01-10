#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QSpinBox>
#include <QSlider>
#include <QDebug>
#include <QLabel>
#include <QComboBox>

#include "../portaudio_engine/scopedPAContext.h"
#include "../portaudio_engine/metronometest.h"


class TestA1MW : public QWidget
{
    Q_OBJECT

public:
    TestA1MW(QWidget* parent = NULL);
    ~TestA1MW();

    void setPortAudioContext(ScopedPAContext* pactx);

public slots:
    void startstopMetronome();
    void changeBPM(int);
    void changeVolume(int);

private:
    ScopedPAContext*    _pactx;

    QGridLayout*    vl;
    QSlider*        sl1, *sl2;
    QSpinBox*       sp1;
    QPushButton*    pb1;
    QLabel*         lb1, *lb2;

    MetronomeTest   mtest;
};

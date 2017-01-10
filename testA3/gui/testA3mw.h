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


class TestA3MW : public QWidget
{
    Q_OBJECT

public:
    TestA3MW(QWidget* parent = NULL);
    ~TestA3MW();

    void setPortAudioContext(ScopedPAContext* pactx);

private:
    ScopedPAContext*    _pactx;
};

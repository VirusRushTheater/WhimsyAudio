#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QComboBox>

#include "../portaudio_engine/scopedPAContext.h"


class TestA1MW : public QWidget
{
    Q_OBJECT

public:
    TestA1MW(QWidget* parent = NULL);
    ~TestA1MW();

    void setPortAudioContext(ScopedPAContext* pactx);

public slots:

private:
    ScopedPAContext*    _pactx;

};

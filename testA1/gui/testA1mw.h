#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QComboBox>

#include "../portaudio_engine/scopedPAContext.h"

struct DeviceInfo
{
    int         index;
    QString     name;
    QString     details;
    bool        is_default_input;
    bool        is_default_output;
};

class TestA1MW : public QWidget
{
    Q_OBJECT

public:
    TestA1MW(QWidget* parent = NULL);
    ~TestA1MW();

    void setPortAudioContext(ScopedPAContext* pactx);
    void getDeviceInfo();

public slots:
    void streamButton();
    void updateDetails(int i);
    void refreshDevices();

private:
    QList<DeviceInfo>   _devices;

    ScopedPAContext*    _pactx;

    QTextEdit*          textfield;
    QPushButton*        pushbutton, *refreshbutton;
    QGridLayout*        mainlayout;
    QComboBox*          devicecombo;
};

#include "testA1mw.h"

TestA1MW::TestA1MW(QWidget *parent)
{
    textfield =     new QTextEdit("Test for humble purposes. No console is needed anymore.");
    mainlayout =    new QGridLayout();
    pushbutton =    new QPushButton("Start/stop metronome!");
    refreshbutton = new QPushButton("Refresh audio devices");
    devicecombo =   new QComboBox();

    mainlayout->addWidget(devicecombo, 0, 0);
    mainlayout->addWidget(textfield, 1, 0);
    mainlayout->addWidget(refreshbutton, 2, 0);
    mainlayout->addWidget(pushbutton, 3, 0);
    this->setLayout(mainlayout);

    getDeviceInfo();

    QObject::connect(devicecombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDetails(int)));
    QObject::connect(pushbutton, SIGNAL(clicked(bool)), this, SLOT(streamButton()));
    QObject::connect(refreshbutton, SIGNAL(clicked(bool)), this, SLOT(refreshDevices()));
}

TestA1MW::~TestA1MW()
{
    delete textfield;
    delete pushbutton;
    delete mainlayout;
}

void TestA1MW::setPortAudioContext(ScopedPAContext* pactx)
{
    _pactx = pactx;
}

void TestA1MW::streamButton()
{
    qDebug() << "Button pressed!";
}

void TestA1MW::getDeviceInfo()
{
    int                     devcount, devindex;
    DeviceInfo              devtemp;
    const PaDeviceInfo*     dinfo;

    QTextStream             devdetails_str;

    _devices.clear();

    devcount = Pa_GetDeviceCount();
    if(devcount < 0)
    {
        return;
    }

    for(devindex = 0; devindex < devcount; devindex++)
    {
        devtemp.index =         devindex;
        dinfo =                 Pa_GetDeviceInfo(devindex);
        devtemp.name =          QString(dinfo->name);

        devtemp.is_default_input =      Pa_GetDefaultInputDevice() == devindex;
        devtemp.is_default_output =     Pa_GetDefaultOutputDevice() == devindex;

        devtemp.details =       QString();
        devdetails_str.setString(&devtemp.details);

        devdetails_str << "[" << devtemp.name << "]\n";

        if(devtemp.is_default_input)
            devdetails_str << "[Default input]\n";
        if(devtemp.is_default_output)
            devdetails_str << "[Default output]\n";

        devdetails_str << "Host API: " << Pa_GetHostApiInfo(dinfo->hostApi)->name << "\n";
        devdetails_str << "Max input channels: " << dinfo->maxInputChannels << "\n";
        devdetails_str << "Max output channels: " << dinfo->maxOutputChannels << "\n\n";
        if(dinfo->defaultLowInputLatency >= 0)
        {
            devdetails_str << "Default low input latency: " << dinfo->defaultLowInputLatency * 1000.0f << " ms\n";
            devdetails_str << "Default high input latency: " << dinfo->defaultHighInputLatency * 1000.0f << " ms\n";
        }
        else
            devdetails_str << "Not an input device.\n";

        if(dinfo->defaultLowOutputLatency >= 0)
        {
            devdetails_str << "Default low output latency: " << dinfo->defaultLowOutputLatency * 1000.0f << " ms\n";
            devdetails_str << "Default high output latency: " << dinfo->defaultLowOutputLatency * 1000.0f << " ms\n";
        }
        else
            devdetails_str << "Not an output device.\n";

        devicecombo->addItem(devtemp.name);

        _devices.append(devtemp);
    }

}

void TestA1MW::updateDetails(int i)
{
    if(i < _devices.count())
        textfield->setText(_devices.at(i).details);
}

void TestA1MW::refreshDevices()
{
    devicecombo->clear();
}

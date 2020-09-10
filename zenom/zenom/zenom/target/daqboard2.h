#ifndef DAQBOARD2_H
#define DAQBOARD2_H

#include "board.h"
#include <iostream>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

using namespace std;
class DaqBoard2 : public board
{
    Q_OBJECT
public:
    DaqBoard2(QObject *parent = nullptr);
    virtual void init();
    virtual void start();
    virtual void stop();
    virtual void pause();
    virtual void resume();
    virtual void reset();
    //when you enable and I/O you will get and ID number
    //ID = -1 is invalid
    virtual int enableInput(QString name);
    virtual int enableOutput(QString name);
    //if you want to acces an I/O you should supply this ID
    virtual double getInput(int id);
    virtual void setOutput(int id, double value);
    virtual void openSettingsDialog();
    virtual void clear();

public slots:
    virtual void serialSync();

protected slots:
    virtual void on_serial_read();

private:
    #pragma pack(push, 1)
    typedef struct {
        uint16_t adc1;
        uint16_t adc2;
    } adc_msg_t;
    #pragma pack(pop)

    const double pi = 3.14159265359;

    QReadWriteLock adc_lock;
    char dummy[4] = {0,0,0,0};
    adc_msg_t adc;

};

#endif // DAQBOARD2_H

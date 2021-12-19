#ifndef DAQBOARD1_H
#define DAQBOARD1_H

#include "board.h"
#include <iostream>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

using namespace std;
class DaqBoard1 : public board
{
    Q_OBJECT
public:
    DaqBoard1(QObject *parent = nullptr);
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
        int16_t enc1;
        int16_t enc2;
    } enc_msg_t;
    #pragma pack(pop)

    #pragma pack(push, 1)
    typedef struct {
        uint16_t dac1;
        uint16_t dac2;
    } dac_msg_t;
    #pragma pack(pop)

    const double pi = 3.14159265359;

    QReadWriteLock enc_lock;
    QReadWriteLock dac_lock;
    enc_msg_t enc;
    dac_msg_t dac;

};

#endif // DAQBOARD1_H

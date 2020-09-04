#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSerialPort>
#include <list>
#include "threadedserial.h"

using namespace std;

class board : public QObject
{
    Q_OBJECT
public:
    board();
    virtual ~board();
    virtual void setComPort(QString name) = 0;
    virtual void setFrequency(int freq);
    virtual QStringList getInputList();
    virtual QStringList getOutputList();
    virtual void init() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void reset() = 0;
    //when you enable and I/O you will get and ID number
    //ID = -1 is invalid
    virtual int enableInput(QString name) = 0;
    virtual int enableOutput(QString name) = 0;
    //if you want to acces an I/O you should supply this ID
    virtual double getInput(int id) = 0;
    virtual void setOutput(int id, double value) = 0;
    virtual void syncOutputs() = 0;
    virtual void openSettingsDialog() = 0;
    QString name;

signals:
    void open(QString portName, qint32 baudrate);
    void isOpen();
    void close();
    void clear();
    void write(QByteArray data);

protected:
    void serialOpen(QString portName, qint32 baudrate);
    void serialIsOpen();
    void serialClose();
    void serialClear();
    void serialWrite(QByteArray data);
    ThreadedSerial *mSerial;
    QByteArray mSerialBuf;
    double mFreq;

    QStringList inputs;
    QStringList outputs;
};

#endif // BOARD_H

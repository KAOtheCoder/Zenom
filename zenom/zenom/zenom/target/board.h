#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSerialPort>
#include <list>

using namespace std;

//This is an abstract class for target board implementation
//it can run with QT's main event loop or it can run multithreaded via BoardWrapper class.
class board : public QObject
{
    Q_OBJECT
public:
    board(QObject *parent = nullptr);
    virtual ~board();
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
    virtual void clear() = 0;
    virtual quint32 getMissedReads();
    QString name;

//All slots will be executed by the event loop that this object belongs to
public slots:
    //this code should run in the target thread.
    //So make it slot in order to run in new event loop
    void serialOpen(QString portName);

protected slots:
    virtual void on_serial_read() = 0;

protected:
    QSerialPort mSerial;
    QByteArray mSerialBuf;
    double mFreq;
    qint32 baudrate;

    QStringList inputs;
    QStringList outputs;

    quint32 missCnt;
};

#endif // BOARD_H

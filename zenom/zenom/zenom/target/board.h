#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QString>
#include <QSerialPort>
#include <list>

using namespace std;

class board : public QObject
{
    Q_OBJECT
public:
    board(QObject *parent = Q_NULLPTR);
    virtual ~board();
    virtual void setComPort(string name);
    virtual void setFrequency(int freq);
    virtual list<string> getInputList();
    virtual list<string> getOutputList();
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void reset() = 0;
    virtual bool enableInput(string name) = 0;
    virtual bool enableOutput(string name) = 0;
    virtual double getInput(string name) = 0;
    virtual void setOutput(string name, double value) = 0;
    virtual void syncOutputs() = 0;
    virtual void openSettingsDialog() = 0;


protected:
    QSerialPort mSerial;
    QByteArray mSerialBuf;
    double mFreq;

    list<string> inputs;
    list<string> outputs;
};

#endif // BOARD_H

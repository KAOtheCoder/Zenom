#include "board.h"

board::board() :
    QObject (nullptr)
{
    name = "not specified";
    mSerial = new ThreadedSerial();
    moveToThread(mSerial);

    connect(this, SIGNAL(open(QString, qint32)), mSerial, SLOT(open(QString, qint32)));
    connect(this, SIGNAL(isOpen()), mSerial, SLOT(isOpen()));
    connect(this, SIGNAL(close()), mSerial, SLOT(close()));
    connect(this, SIGNAL(clear()), mSerial, SLOT(clear()));
    connect(this, SIGNAL(write(QByteArray)), mSerial, SLOT(write(QByteArray)));
}

board::~board() = default;

void board::serialOpen(QString portName, qint32 baudrate){
    emit open(portName, baudrate);
}
void board::serialIsOpen(){
    emit isOpen();
}
void board::serialClose(){
    emit close();
}
void board::serialClear(){
    emit clear();
}
void board::serialWrite(QByteArray data){
    emit write(data);
}

void board::setFrequency(int freq){
    mFreq = 1.0 / freq;
}

QStringList board::getInputList(){
    return inputs;
}

QStringList board::getOutputList(){
    return outputs;
}

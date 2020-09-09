#include "board.h"

board::board(QObject *parent)
    : QObject (parent)
    , mSerial (this)
    , baudrate(0)
    , missCnt(0)
{
    name = "not specified";
    connect(&mSerial, SIGNAL(readyRead()), this, SLOT(on_serial_read()));
}

board::~board() = default;

void board::serialOpen(QString portName){
    if(mSerial.portName() != portName){
        if(mSerial.isOpen())
            mSerial.close();
        mSerial.setBaudRate(baudrate);
        mSerial.setPortName(portName);
        mSerial.open(QIODevice::ReadWrite);
    }
    if(!mSerial.isOpen()){
        mSerial.setBaudRate(baudrate);
        mSerial.setPortName(portName);
        mSerial.open(QIODevice::ReadWrite);
    }
    clear();
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

quint32 board::getMissedReads()
{
    return missCnt;
}

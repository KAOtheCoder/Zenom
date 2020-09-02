#include "board.h"

board::board(QObject *parent) :
    QObject (parent)
{
    name = "not specified";
}

board::~board() = default;

void board::setComPort(QString name){
    mSerial.setPortName(name);
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

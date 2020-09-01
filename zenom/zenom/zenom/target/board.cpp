#include "board.h"

board::board(QObject *parent) :
    QObject (parent)
{
}

board::~board() = default;

void board::setComPort(string name){
    mSerial.setPortName(QString::fromStdString(name));
}

void board::setFrequency(int freq){
    mFreq = 1.0 / freq;
}

list<string> board::getInputList(){
    return inputs;
}

list<string> board::getOutputList(){
    return outputs;
}

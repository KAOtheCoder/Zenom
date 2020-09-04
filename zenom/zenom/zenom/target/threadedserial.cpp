#include "threadedserial.h"

ThreadedSerial::ThreadedSerial()
    :QThread (nullptr),
     mSerial (nullptr)
{
    start(QThread::TimeCriticalPriority);
    moveToThread(this);
}

ThreadedSerial::~ThreadedSerial()
{
    quit();
    wait();
}


void ThreadedSerial::run()
{
    mSerial = new QSerialPort(this);
    connect(mSerial, SIGNAL(readyRead()), this, SLOT(on_serial_read()));
    exec();
}

void ThreadedSerial::open(QString portName, qint32 baudrate)
{
    if(mSerial == nullptr) return;
    if(mSerial->portName() != portName){
        if(mSerial->isOpen())
            mSerial->close();
        mSerial->setBaudRate(baudrate);
        mSerial->setPortName(portName);
        mSerial->open(QIODevice::ReadWrite);
    }
    if(!mSerial->isOpen()){
        mSerial->setBaudRate(baudrate);
        mSerial->setPortName(portName);
        mSerial->open(QIODevice::ReadWrite);
    }
    return;
}
void ThreadedSerial::close(){
    if(mSerial == nullptr) return;
    if(!mSerial->isOpen()) return;
    mSerial->close();
}
void ThreadedSerial::clear()
{
    if(mSerial == nullptr) return;
    if(!mSerial->isOpen()) return;
    mSerial->readAll();
    mSerial->clear();
}
void ThreadedSerial::write(QByteArray data){
    if(mSerial == nullptr) return;
    if(!mSerial->isOpen()) return;
    mSerial->write(data);
    mSerial->flush();
}

void ThreadedSerial::on_serial_read()
{
    emit data_ready(mSerial->readAll());
}

#include "daqboard1.h"

DaqBoard1::DaqBoard1() :
    board()
{
    name = "DaqBoard1";
    reset();

    inputs.insert(inputs.end(), "ENC1");
    inputs.insert(inputs.end(), "ENC2");

    outputs.insert(outputs.end(), "DAC1");
    outputs.insert(outputs.end(), "DAC2");

    connect(mSerial, SIGNAL(data_ready(QByteArray)), SLOT(on_serial_read(QByteArray)));
}
void DaqBoard1::setComPort(QString name){
    serialOpen(name, 1000000);
    clear();
}

void DaqBoard1::clear(){
    enc.enc1 = 0;
    enc.enc2 = 0;
    dac.dac1 = 2048;
    dac.dac2 = 2048;
    mSerialBuf.clear();
    serialClear();
}

void DaqBoard1::init()
{
    clear();
}
void DaqBoard1::start()
{
    clear();
}
void DaqBoard1::stop()
{
    dac.dac1 = 2048;
    dac.dac2 = 2048;
    syncOutputs();
    clear();
}
void DaqBoard1::pause()
{
    dac.dac1 = 2048;
    dac.dac2 = 2048;
    syncOutputs();
}
void DaqBoard1::resume()
{
}
void DaqBoard1::reset()
{
    clear();
}
int DaqBoard1::enableInput(QString name)
{
    if(name == "ENC1") return 1;
    if(name == "ENC2") return 2;
    return -1;
}
int DaqBoard1::enableOutput(QString name)
{
    if(name == "DAC1") return 3;
    if(name == "DAC2") return 4;
    return -1;
}
double DaqBoard1::getInput(int id)
{
    if(id == 1){
        return (double)(enc.enc1)*2.0*pi/4096;
    }
    else if (id == 2) {
        return (double)(enc.enc2)*1.0*pi/4096;
    }
}
void DaqBoard1::setOutput(int id, double value)
{
    if(value > 10) value = 10;
    if(value < -10) value = -10;
    if(id == 3){
        dac.dac1 = (uint16_t)(value*2048.0/10.0+2048.0);
    }
    else if (id == 4) {
        dac.dac2 = (uint16_t)(value*2048.0/10.0+2048.0);
    }
}
void DaqBoard1::syncOutputs()
{
    serialWrite(QByteArray::fromRawData((char*)&dac, sizeof(dac_msg_t)));
}
void DaqBoard1::openSettingsDialog()
{
    //do nothing
}
void DaqBoard1::on_serial_read(QByteArray data){
    mSerialBuf.append(data);
    while(mSerialBuf.size() >= sizeof (enc_msg_t)){
        if(mSerialBuf.size() == sizeof (enc_msg_t)){
            memcpy(&enc, mSerialBuf.data(), sizeof(enc_msg_t));
            mSerialBuf.remove(0,sizeof (enc_msg_t));
        }
        else {
            //debug
            cout << mSerialBuf.size()-4 << endl;
            mSerialBuf.remove(0,mSerialBuf.size()-sizeof (enc_msg_t));
        }
    }
}

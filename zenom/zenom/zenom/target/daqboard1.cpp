#include "daqboard1.h"

DaqBoard1::DaqBoard1()
{
    mSerial.setBaudRate(1000000);

    inputs.insert(inputs.end(), "ENC1");
    inputs.insert(inputs.end(), "ENC2");

    outputs.insert(outputs.end(), "DAC1");
    outputs.insert(outputs.end(), "DAC2");

    connect(&mSerial, SIGNAL(readyRead()), SLOT(on_serial_read()));
}
void DaqBoard1::start()
{
    if(!mSerial.open(QIODevice::ReadWrite)){
        cout << "[TARGET] Couldn't open serial port!";
    }
}
void DaqBoard1::stop()
{
    mSerial.close();
}
void DaqBoard1::pause()
{
    //do nothing
}
void DaqBoard1::resume()
{
    //do nothing
}
void DaqBoard1::reset()
{
    //do nothing
}
bool DaqBoard1::enableInput(string name)
{
    return true;
}
bool DaqBoard1::enableOutput(string name)
{
    return true;
}
double DaqBoard1::getInput(string name)
{
    if(name == "ENC1"){
        return (double)(enc.enc1)*2.0*pi/4096;
    }
    else if (name == "ENC2") {
        return (double)(enc.enc2)*1.0*pi/4096;
    }
}
void DaqBoard1::setOutput(string name, double value)
{
    if(name == "DAC1"){
        dac.dac1 = (uint16_t)(value*2048.0/10.0+2048.0);
    }
    else if (name == "DAC2") {
        dac.dac2 = (uint16_t)(value*2048.0/10.0+2048.0);
    }
}
void DaqBoard1::syncOutputs()
{
    mSerial.write((char*)&dac, sizeof(dac_msg_t));
    mSerial.flush();
}
void DaqBoard1::openSettingsDialog()
{
    //do nothing
}
void DaqBoard1::on_serial_read(){
    mSerialBuf.append(mSerial.readAll());
    while(mSerialBuf.size() >= sizeof (enc_msg_t)){
        if(mSerialBuf.size() == sizeof (enc_msg_t)){
            memcpy(&enc, mSerialBuf.data(), sizeof(enc_msg_t));
            mSerialBuf.remove(0,sizeof (enc_msg_t));
        }
        else {
            mSerialBuf.remove(0,1);
        }
    }
}

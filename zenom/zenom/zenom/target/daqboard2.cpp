//#include "daqboard2.h"

//DaqBoard2::DaqBoard2(QObject *parent)
//    : board(parent)
//{
//    name = "DaqBoard2";
//    baudrate = 1000000;
//    reset();

//    inputs.insert(inputs.end(), "ADC1");
//    inputs.insert(inputs.end(), "ADC2");

//    outputs.insert(outputs.end(), "SERVO1");
//    outputs.insert(outputs.end(), "SERVO2");
//    outputs.insert(outputs.end(), "SERVO3");
//}

//void DaqBoard2::clear(){
//    adc.adc1 = 0;
//    adc.adc2 = 0;
//    missCnt = 0;
//    serialSync();
//    mSerialBuf.clear();
//    if(mSerial.isOpen())
//        mSerial.clear();
//}

//void DaqBoard2::init()
//{
//    clear();
//}
//void DaqBoard2::start()
//{
//    clear();
//}
//void DaqBoard2::stop()
//{
//    clear();
//}
//void DaqBoard2::pause()
//{
//    serialSync();
//}
//void DaqBoard2::resume()
//{
//    //do nothing
//}
//void DaqBoard2::reset()
//{
//    clear();
//}
//int DaqBoard2::enableInput(QString name)
//{
//    if(name == "ADC1") return 1;
//    if(name == "ADC2") return 2;
//    return -1;
//}
//int DaqBoard2::enableOutput(QString name)
//{
//    if(name == "SERVO1") return 3;
//    if(name == "SERVO2") return 4;
//    if(name == "SERVO3") return 5;
//    return -1;
//}
//double DaqBoard2::getInput(int id)
//{
//    if(id == 1){
//        QReadLocker locker(&adc_lock);
//        return (double)(adc.adc1)*3.3/4096.0;
//    }
//    else if (id == 2) {
//        QReadLocker locker(&adc_lock);
//        return (double)(adc.adc2)*3.3/4096.0;
//    }
//    else return 0;
//}
//void DaqBoard2::setOutput(int id, double value)
//{
//    if(value < 0) value = 0;
//    if(value > 180) value = 180;
//    if(id == 3){
//        QWriteLocker locker(&servo_lock);
//        servo.servo1 = (value/180.0)*(servo_max-servo_min)+servo_min;
//    }
//    else if(id == 4){
//        QWriteLocker locker(&servo_lock);
//        servo.servo2 = (value/180.0)*(servo_max-servo_min)+servo_min;
//    }
//    else if(id == 5){
//        QWriteLocker locker(&servo_lock);
//        servo.servo3 = (value/180.0)*(servo_max-servo_min)+servo_min;
//    }
//}
//void DaqBoard2::serialSync()
//{
//    QReadLocker locker(&servo_lock);
//    mSerial.write(QByteArray::fromRawData((char*)&servo, sizeof(servo_msg_t)));
//}
//void DaqBoard2::openSettingsDialog()
//{
//    //do nothing
//}
//void DaqBoard2::on_serial_read(){
//    mSerialBuf.append(mSerial.readAll());
//    while(mSerialBuf.size() >= sizeof (adc_msg_t)){
//        if(mSerialBuf.size() == sizeof (adc_msg_t)){
//            QWriteLocker locker(&adc_lock);
//            memcpy(&adc, mSerialBuf.data(), sizeof(adc_msg_t));
//            mSerialBuf.remove(0,sizeof (adc_msg_t));
//        }
//        else {
//            missCnt += (mSerialBuf.size()-4)/4;
//            mSerialBuf.remove(0,mSerialBuf.size()-sizeof (adc_msg_t));
//        }
//    }
//}

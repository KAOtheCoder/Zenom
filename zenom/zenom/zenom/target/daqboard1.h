#ifndef DAQBOARD1_H
#define DAQBOARD1_H

#include "board.h"
#include <iostream>

using namespace std;
class DaqBoard1 : public board
{
public:
    DaqBoard1();
    virtual void start();
    virtual void stop();
    virtual void pause();
    virtual void resume();
    virtual void reset();
    virtual bool enableInput(string name);
    virtual bool enableOutput(string name);
    virtual double getInput(string name);
    virtual void setOutput(string name, double value);
    virtual void syncOutputs();
    virtual void openSettingsDialog();

private slots:
    void on_serial_read();

private:
    #pragma pack(push, 1)
    typedef struct {
        int16_t enc1;
        int16_t enc2;
    } enc_msg_t;
    #pragma pack(pop)

    #pragma pack(push, 1)
    typedef struct {
        uint16_t dac1;
        uint16_t dac2;
    } dac_msg_t;
    #pragma pack(pop)

    const double pi = 3.14159265359;

    enc_msg_t enc;
    dac_msg_t dac;
};

#endif // DAQBOARD1_H

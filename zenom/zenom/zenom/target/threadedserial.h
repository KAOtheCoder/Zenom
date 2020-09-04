#ifndef THREADEDSERIAL_H
#define THREADEDSERIAL_H

#include <QThread>
#include <QSerialPort>

class ThreadedSerial : public QThread
{
    Q_OBJECT
public:
    ThreadedSerial();
    ~ThreadedSerial();
    void run();

public slots:
    void open(QString portName, qint32 baudrate);
    void close();
    void clear();
    void write(QByteArray data);

signals:
    void data_ready(QByteArray data);

private slots:
    void on_serial_read();

private:
    QSerialPort *mSerial;
};

#endif // THREADEDSERIAL_H

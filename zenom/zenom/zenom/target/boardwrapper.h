#ifndef BOARDWRAPPER_H
#define BOARDWRAPPER_H

#include "board.h"
#include <QThread>

//This class creates new event loop for target board.
//SerialPort access and data processing will be executed in this new thread.
class BoardWrapper : public QObject
{
    Q_OBJECT
public:
    BoardWrapper(QObject *parent, board *targetBoard);
    QThread boardThread;
    board *target;
    void serialOpen(QString portName);

signals:
    void open(QString portName);

};

#endif // BOARDWRAPPER_H

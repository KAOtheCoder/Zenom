#ifdef BOARDWRAPPER_H
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
    ~BoardWrapper();
    QThread boardThread;
    board *target;
    void serialOpen(QString portName);
    void serialSync();

signals:
    void open(QString portName);
    void sync();

};

#endif // BOARDWRAPPER_H

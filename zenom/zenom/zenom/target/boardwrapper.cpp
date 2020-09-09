#include "boardwrapper.h"

BoardWrapper::BoardWrapper(QObject *parent, board *targetBoard)
    : QObject (parent)
    , boardThread (this)
    , target (targetBoard)
{
    //we will use a new event loop in order not to be affacted from gui
    target->moveToThread(&boardThread);
    connect(this, SIGNAL(open(QString)), target, SLOT(serialOpen(QString)));
    connect(&boardThread, SIGNAL(finished()), target, SLOT(deleteLater()));
    boardThread.start(QThread::TimeCriticalPriority);
}

//this code should run in the target thread.
//So use signal slot mechanism to run it under new threads event loop
void BoardWrapper::serialOpen(QString portName){
    emit open(portName);
}

BoardWrapper::~BoardWrapper(){
    boardThread.quit();
    boardThread.wait();
}

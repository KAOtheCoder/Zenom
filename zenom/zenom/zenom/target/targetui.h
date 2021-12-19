#ifndef DAQ_H
#define DAQ_H

#include <QDialog>
#include <QStatusBar>
#include <datarepository.h>
#include "comboboxitemdelegate.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include "targettask.h"
#include "boardwrapper.h"
#include "daqboard1.h"
#include "daqboard2.h"

namespace Ui {
class TargetUI;
}

class TargetUI : public QDialog
{
    Q_OBJECT

public:

    explicit TargetUI(QWidget *parent = nullptr);
    ~TargetUI();

    void sendStateRequest(StateRequest pRequest);

    void setFrequency(double freq);
    int doloop();
    void saveSettings( QSettings& pSettings );
    void loadSettings( QSettings& pSettings );
    void tick();
    State state;

private slots:
    void on_cbBoards_currentIndexChanged(const QString &arg1);

    void on_pb_reset_clicked();

    void on_pb_settings_clicked();

private:
    Ui::TargetUI *ui;
    QStatusBar *mStatusBar;
    const ControlVariableList& cntrVariables;
    const LogVariableList& logVariables;
    int tickCnt;
    int dacTable[8];
    int encTable[8];
    int adcTable[8];
    int out[4];
    int in[4];
    double mFreq;

    TargetTask *mLoopTask;
    void loop_start();
    void loop_end();

    list<BoardWrapper*> boards;
    BoardWrapper *selectedBoard;
    void registerBoard(board *boardPtr);
    void updateTables();
    void updateComPortList();

    typedef struct{
        uint varIndex;
        int row;
        int col;
        int targetID;
    } lookup_entry_t;

    QStringList variableName(Variable *var);
    lookup_entry_t createLookupEntry(QString varName, int targetID);
    void updateLookupTable();
    QList<lookup_entry_t> input_lookup;
    QList<lookup_entry_t> output_lookup;

    void setControlsStatus(bool stat);
};

#endif // DAQ_H

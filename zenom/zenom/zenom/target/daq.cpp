#include "daq.h"
#include "ui_daq.h"

daq::daq(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::daq),
    cntrVariables(DataRepository::instance()->controlVariables()),
    logVariables(DataRepository::instance()->logVariables()),
    tickCnt(0)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(mStatusBar = new QStatusBar());
    mStatusBar->setMaximumHeight(20);

    updateComPortList();
    registerBoard(new DaqBoard1(this));
    updateTables();
}

daq::~daq()
{
    delete ui;
}

void daq::registerBoard(board *boardPtr){
    boards.insert(boards.end(), boardPtr);
    ui->cbBoards->addItem(boardPtr->name);
    if(boards.size() == 1){
        selectedBoard = boardPtr;
    }
}


int daq::doloop(){
    tickCnt++;
    if(state == RUNNING){
        for (const auto i : input_lookup){
            cntrVariables[i.varIndex]->setHeapElement(i.row, i.col, selectedBoard->getInput(i.targetID));
        }

        for (const auto i : output_lookup){
            if(logVariables[i.varIndex]->isHeapValid()){
                selectedBoard->setOutput(i.targetID, logVariables[i.varIndex]->lastHeapElement(i.row, i.col));
            }
        }
        selectedBoard->syncOutputs();
    } //running
    return 0;
}
void daq::updateComPortList()
{
    ui->cbPorts->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->cbPorts->addItem(info.portName());
    }

}

void daq::sendStateRequest(StateRequest pRequest)
{
    if(pRequest == R_INIT){
        state = STOPPED;
        mStatusBar->showMessage("Init");

        setControlsStatus(true);
        updateTables();
        selectedBoard->init();
        selectedBoard->setComPort(ui->cbPorts->currentText());
    }
    else if(pRequest == R_START){
        state = RUNNING;
        mStatusBar->showMessage("START");

        updateLookupTable();
        setControlsStatus(false);
        selectedBoard->setComPort(ui->cbPorts->currentText());
        selectedBoard->start();
        loop_start();
    }
    else if(pRequest == R_PAUSE){
        state = PAUSED;
        mStatusBar->showMessage("PAUSED");

        setControlsStatus(true);
        selectedBoard->pause();
    }
    else if(pRequest == R_RESUME){
        state = RUNNING;
        mStatusBar->showMessage("RESUMED");

        setControlsStatus(false);
        selectedBoard->resume();
    }
    else if(pRequest == R_STOP){
        state = STOPPED;
        mStatusBar->showMessage("STOPPED");

        tickCnt = 0;
        setControlsStatus(true);
        selectedBoard->stop();
        loop_end();

        //set 0 all inputs.
        for (const auto i : input_lookup){
            cntrVariables[i.varIndex]->setHeapElement(i.row, i.col, 0);
        }
    }
    else if(pRequest == R_TERMINATE){
        state = TERMINATED;
        mStatusBar->showMessage("TERMINATED");

        setControlsStatus(true);
        selectedBoard->reset();
    }
}

QStringList daq::variableName(Variable *var){
    //varName
    //varName[colmn]
    //varName[row][colmn]
    QStringList ret;
    if(var->col() == 1 && var->row() == 1){
        ret.append(QString::fromStdString(var->name()));
        return ret;
    }
    if(var->row() == 1){
        for(uint j=0; j<var->col(); j++){
            ret.append(QString::fromStdString(var->name()) + "[" + QString::number(j) + "]");
        }
        return ret;
    }
    for(uint j=0; j<var->row(); j++){
        for(uint k=0; k<var->col(); k++){
            ret.append(
                    QString::fromStdString(var->name()) + "[" + QString::number(j) + "]" +
                    "[" + QString::number(k) + "]");
        }
    }
    return ret;
}
daq::lookup_entry_t daq::createLookupEntry(QString varName, int targetID)
{
    lookup_entry_t entry;
    QString name;

    entry.targetID = targetID;

    if(varName.count('[') == 2){
        int firstBracketS = varName.indexOf('[');
        int firstBracketE = varName.indexOf(']');
        int secBracketS = varName.indexOf('[',firstBracketS+1);
        int secBracketE = varName.indexOf(']',firstBracketE+1);
        name = varName.left(firstBracketS);
        entry.row = varName.mid(firstBracketS+1, firstBracketE-firstBracketS-1).toInt();
        entry.col = varName.mid(secBracketS+1, secBracketE-secBracketE-1).toInt();
    }
    else if(varName.count('[') == 1){
        int firstBracketS = varName.indexOf('[');
        int firstBracketE = varName.indexOf(']');
        name = varName.left(firstBracketS);
        entry.col = varName.mid(firstBracketS+1, firstBracketE-firstBracketS-1).toInt();
        entry.row = 0;
    }
    else {
        name = varName;
        entry.row = 0;
        entry.col = 0;
    }
    for(uint j=0; j<cntrVariables.size(); j++){
        if(QString::fromStdString(cntrVariables.at(j)->name()) == name){
            entry.varIndex = j;
            return entry;
        }
    }
    for(uint j=0; j<logVariables.size(); j++){
        if(QString::fromStdString(logVariables.at(j)->name()) == name){
            entry.varIndex = j;
            return entry;
        }
    }
    return entry;
}
void daq::updateLookupTable()
{
    input_lookup.clear();
    output_lookup.clear();

    for(int i=0; i<ui->tblIn->rowCount(); i++){
        if(ui->tblIn->item(i,1) == nullptr) continue;
        QString varName = ui->tblIn->item(i,1)->text();
        QString channel = ui->tblIn->item(i,0)->text();
        if( varName != ""){
            input_lookup.append(createLookupEntry(varName, selectedBoard->enableInput(channel)));
        }
    }

    for(int i=0; i<ui->tblOut->rowCount(); i++){
        if(ui->tblOut->item(i,1) == nullptr) continue;
        QString varName = ui->tblOut->item(i,1)->text();
        QString channel = ui->tblOut->item(i,0)->text();
        if( varName != ""){
            output_lookup.append(createLookupEntry(varName, selectedBoard->enableOutput(channel)));
        }
    }
}

void daq::updateTables(){
    ui->tblOut->clear();
    ui->tblIn->clear();

    if(ui->tblOut->itemDelegateForColumn(1) != nullptr)
        delete ui->tblOut->itemDelegateForColumn(1);

    if(ui->tblIn->itemDelegateForColumn(1) != nullptr)
        delete ui->tblIn->itemDelegateForColumn(1);

    QStringList inputs = selectedBoard->getInputList();
    QStringList outputs = selectedBoard->getOutputList();

    ui->tblIn->setRowCount(inputs.length());
    for(int i=0; i<inputs.length(); i++){
        QTableWidgetItem *item = new QTableWidgetItem(inputs[i]);
        ui->tblIn->setItem(i,0,item);
    }

    ui->tblOut->setRowCount(outputs.length());
    for(int i=0; i<outputs.length(); i++){
        QTableWidgetItem *item = new QTableWidgetItem(outputs[i]);
        ui->tblOut->setItem(i,0,item);
    }

    QStringList logVarList = QStringList();
    QStringList cntVarList = QStringList();
    for(uint i=0; i<logVariables.size(); i++){
        logVarList.append(variableName(logVariables.at(i)));
    }
    for(uint i=0; i<cntrVariables.size(); i++){
        cntVarList.append(variableName(cntrVariables.at(i)));
    }

    ComboBoxItemDelegate *cbid = new ComboBoxItemDelegate(logVarList, ui->tblOut);
    ui->tblOut->setItemDelegateForColumn(1, cbid);

    ComboBoxItemDelegate *cbid2 = new ComboBoxItemDelegate(cntVarList, ui->tblIn);
    ui->tblIn->setItemDelegateForColumn(1, cbid2);
}


void daq::setFrequency(double freq){
    mFreq = freq;
}

void daq::loop_start(){
    mLoopTask = new TargetTask( this,
                                std::chrono::duration<double>(
                                    1.0 / mFreq
                                    ),
                                "targetLoopTask"
                                );
    mLoopTask->runTask();
}

void daq::loop_end(){
    mLoopTask->requestPeriodicTaskTermination();
    mLoopTask->join();
    delete mLoopTask;
}

void daq::on_cbBoards_currentIndexChanged(const QString &arg1)
{
    updateComPortList();
    for (auto b : boards){
        if(b->name.compare(arg1) == 0){
            selectedBoard = b;
            updateTables();
            cout << "selected board: " << arg1.toStdString() << endl;
            break;
        }
    }
}

void daq::on_pb_reset_clicked()
{
    selectedBoard->reset();
}

void daq::on_pb_settings_clicked()
{
   selectedBoard->openSettingsDialog();
}

void daq::setControlsStatus(bool stat)
{
    ui->tblIn->setEnabled(stat);
    ui->tblOut->setEnabled(stat);
    ui->pb_reset->setEnabled(stat);
    ui->pb_settings->setEnabled(stat);
    ui->cbPorts->setEnabled(stat);
    ui->cbBoards->setEnabled(stat);
}

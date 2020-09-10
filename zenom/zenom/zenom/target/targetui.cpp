#include "targetui.h"
#include "ui_targetui.h"

TargetUI::TargetUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TargetUI),
    cntrVariables(DataRepository::instance()->controlVariables()),
    logVariables(DataRepository::instance()->logVariables()),
    tickCnt(0),
    mLoopTask(nullptr)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(mStatusBar = new QStatusBar());
    mStatusBar->setMaximumHeight(20);

    updateComPortList();
    registerBoard(new DaqBoard1());
    registerBoard(new DaqBoard2());
    updateTables();
}

TargetUI::~TargetUI()
{
    for (auto b : boards){
        delete b;
    }
    delete ui;
}

void TargetUI::registerBoard(board *boardPtr){
    BoardWrapper *wrapper = new BoardWrapper(this, boardPtr);
    boards.insert(boards.end(), wrapper);
    ui->cbBoards->addItem(boardPtr->name);
    if(boards.size() == 1){
        selectedBoard = wrapper;
    }
}


int TargetUI::doloop(){
    tickCnt++;
    if(state == RUNNING){
        for (const auto i : input_lookup){
            cntrVariables[i.varIndex]->setHeapElement(i.row, i.col, selectedBoard->target->getInput(i.targetID));
        }

        for (const auto i : output_lookup){
            if(logVariables[i.varIndex]->isHeapValid()){
                selectedBoard->target->setOutput(i.targetID, logVariables[i.varIndex]->lastHeapElement(i.row, i.col));
            }
        }
        selectedBoard->serialSync();
    } //running
    return 0;
}
void TargetUI::updateComPortList()
{
    ui->cbPorts->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->cbPorts->addItem(info.portName());
    }

}

void TargetUI::sendStateRequest(StateRequest pRequest)
{
    if(pRequest == R_INIT){
        state = STOPPED;

        updateTables();
        selectedBoard->target->init();
    }
    else if(pRequest == R_START){
        state = RUNNING;

        updateLookupTable();
        selectedBoard->serialOpen(ui->cbPorts->currentText());
        selectedBoard->target->start();
        loop_start();
    }
    else if(pRequest == R_PAUSE){
        state = PAUSED;
        selectedBoard->target->pause();
    }
    else if(pRequest == R_RESUME){
        state = RUNNING;
        selectedBoard->target->resume();
    }
    else if(pRequest == R_STOP){
        state = STOPPED;

        tickCnt = 0;
        selectedBoard->target->stop();
        loop_end();

        //set 0 all inputs.
        for (const auto i : input_lookup){
            cntrVariables[i.varIndex]->setHeapElement(i.row, i.col, 0);
        }
    }
    else if(pRequest == R_TERMINATE){
        state = TERMINATED;
        selectedBoard->target->reset();
    }
    //to update ui
    tick();
}

QStringList TargetUI::variableName(Variable *var){
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
TargetUI::lookup_entry_t TargetUI::createLookupEntry(QString varName, int targetID)
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
void TargetUI::updateLookupTable()
{
    input_lookup.clear();
    output_lookup.clear();

    for(int i=0; i<ui->tblIn->rowCount(); i++){
        if(ui->tblIn->item(i,1) == nullptr) continue;
        QString varName = ui->tblIn->item(i,1)->text();
        QString channel = ui->tblIn->item(i,0)->text();
        if( varName != ""){
            input_lookup.append(createLookupEntry(varName, selectedBoard->target->enableInput(channel)));
        }
    }

    for(int i=0; i<ui->tblOut->rowCount(); i++){
        if(ui->tblOut->item(i,1) == nullptr) continue;
        QString varName = ui->tblOut->item(i,1)->text();
        QString channel = ui->tblOut->item(i,0)->text();
        if( varName != ""){
            output_lookup.append(createLookupEntry(varName, selectedBoard->target->enableOutput(channel)));
        }
    }
}

void TargetUI::updateTables(){
    ui->tblOut->clear();
    ui->tblIn->clear();

    if(ui->tblOut->itemDelegateForColumn(1) != nullptr)
        delete ui->tblOut->itemDelegateForColumn(1);

    if(ui->tblIn->itemDelegateForColumn(1) != nullptr)
        delete ui->tblIn->itemDelegateForColumn(1);

    QStringList inputs = selectedBoard->target->getInputList();
    QStringList outputs = selectedBoard->target->getOutputList();

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

    //to be ablte select nothing from dropdown menu
    logVarList.append("");
    cntVarList.append("");
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


void TargetUI::setFrequency(double freq){
    mFreq = freq;
}

void TargetUI::loop_start(){
    mLoopTask = new TargetTask( this,
                                std::chrono::duration<double>(
                                    1.0 / mFreq
                                    ),
                                "targetLoopTask"
                                );
    mLoopTask->runTask();
}

void TargetUI::loop_end(){
    mLoopTask->requestPeriodicTaskTermination();
    mLoopTask->join();
    delete mLoopTask;
    mLoopTask = nullptr;
}

void TargetUI::on_cbBoards_currentIndexChanged(const QString &arg1)
{
    updateComPortList();
    for (auto b : boards){
        if(b->target->name.compare(arg1) == 0){
            selectedBoard = b;
            updateTables();
            cout << "selected board: " << arg1.toStdString() << endl;
            break;
        }
    }
}

void TargetUI::on_pb_reset_clicked()
{
    selectedBoard->target->reset();
}

void TargetUI::on_pb_settings_clicked()
{
   selectedBoard->target->openSettingsDialog();
}

void TargetUI::setControlsStatus(bool stat)
{
    ui->tblIn->setEnabled(stat);
    ui->tblOut->setEnabled(stat);
    ui->pb_reset->setEnabled(stat);
    ui->pb_settings->setEnabled(stat);
    ui->cbPorts->setEnabled(stat);
    ui->cbBoards->setEnabled(stat);
}

void TargetUI::saveSettings( QSettings& pSettings )
{
    pSettings.beginGroup("Target");
    pSettings.setValue("geometry", saveGeometry());
    pSettings.setValue("visible", isVisible());
    pSettings.setValue("board", ui->cbBoards->currentText());
    pSettings.setValue("input_size", ui->tblIn->rowCount());
    for(int i=0; i<ui->tblIn->rowCount(); i++){
        pSettings.beginGroup("input" + QString::number(i));
        if(ui->tblIn->item(i,1) == nullptr)
            pSettings.setValue("var", "");
        else
            pSettings.setValue("var",ui->tblIn->item(i,1)->text() );
        pSettings.endGroup();
    }
    pSettings.setValue("output_size", ui->tblOut->rowCount());
    for(int i=0; i<ui->tblOut->rowCount(); i++){
        pSettings.beginGroup("output" + QString::number(i));
        if(ui->tblOut->item(i,1) == nullptr)
            pSettings.setValue("var", "");
        else
            pSettings.setValue("var",ui->tblOut->item(i,1)->text() );
        pSettings.endGroup();
    }
    pSettings.endGroup();
}

void TargetUI::loadSettings( QSettings& pSettings )
{
    pSettings.beginGroup("Target");
    restoreGeometry( pSettings.value("geometry").toByteArray() );
    setVisible( pSettings.value("visible").toBool() );
    QString boardName = pSettings.value("board").toString();
    int index = ui->cbBoards->findText(boardName);
    if ( index != -1 ) { // -1 for not found
       ui->cbBoards->setCurrentIndex(index);
    }
    for (auto b : boards){
        if(b->target->name.compare(boardName) == 0){
            selectedBoard = b;
            updateTables();
            break;
        }
    }
    int insize = pSettings.value("input_size", 0).toInt();
    for ( int i = 0; i < insize; ++i )
    {
        pSettings.beginGroup( QString("input") + QString::number(i) );
        QString varName = pSettings.value("var").toString();
        QTableWidgetItem *item = new QTableWidgetItem(varName);
        ui->tblIn->setItem(i,1,item);
        pSettings.endGroup();
    }
    int outsize = pSettings.value("output_size", 0).toInt();
    for ( int i = 0; i < outsize; ++i )
    {
        pSettings.beginGroup( QString("output") + QString::number(i) );
        QString varName = pSettings.value("var").toString();
        QTableWidgetItem *item = new QTableWidgetItem(varName);
        ui->tblOut->setItem(i,1,item);
        pSettings.endGroup();
    }
    pSettings.endGroup();
}

void TargetUI::tick()
{
    switch (state) {
    case STOPPED:
    case PAUSED:
    case TERMINATED:
    case CRASHED:
        setControlsStatus(true);
        break;
    case RUNNING:
        QString msg;
        msg += "Read Miss: ";
        msg += QString::number(selectedBoard->target->getMissedReads());
        if(mLoopTask != nullptr){
            msg += "\t\t";
            msg += "Overruns: ";
            msg += QString::number(mLoopTask->overruns());
        }
        setControlsStatus(false);
        mStatusBar->showMessage(msg);
        break;
    }
}

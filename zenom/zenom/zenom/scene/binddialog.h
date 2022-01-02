#ifndef BINDDIALOG_H
#define BINDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QButtonGroup>

#include "datarepository.h"
#include "propertytracker.h"

class BindDialog : public QDialog
{
    Q_OBJECT

public:
    BindDialog(const LogVariableList& pLogVariables, QWidget* pParent = nullptr);

public slots:
    void openDialog(PropertyTracker* pPropertyTracker);

signals:
    void bindTriggered(PropertyTracker* pPropertyTracker, LogVariable* pLogVariable);
    void unbindTriggered(PropertyTracker* pPropertyTracker);

private slots:
    void bind();
    void unbind();
    void updateBindButtonEnabled();

private:
    LogVariableList mLogVariables;
    PropertyTracker* mPropertyTracker;
    QLabel* mTypeLabel;
    QLabel* mNameLabel;
    QButtonGroup* mLogVariablesButtonGroup;
    QPushButton* mBindButton;
};

#endif // BINDDIALOG_H

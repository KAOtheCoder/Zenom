#ifndef PROPERTYINSPECTORITEM_H
#define PROPERTYINSPECTORITEM_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "propertytracker.h"
#include "inspectorpage.h"

class PropertyInspectorItem : public QObject
{
    Q_OBJECT

public:
    PropertyInspectorItem(InspectorPage* pInspector, const QMetaProperty& pProperty);

    void updateValue();

signals:
    void bindDialogRequested(PropertyTracker*);

private slots:
    void setValue();

private:
    PropertyTracker mPropertyTracker;
    QLabel* mTypeLabel;
    QLabel* mNameLabel;
    QLineEdit* mValueLineEdit;
    QPushButton* mBindButton;
};

#endif // PROPERTYINSPECTORITEM_H

#ifndef INSPECTORPAGE_H
#define INSPECTORPAGE_H

#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>

#include "propertywidget.h"

class PropertyWidget;

class InspectorPage : public QScrollArea
{
    Q_OBJECT

public:
    InspectorPage(QObject* pObjectToInspect);

    inline QObject* object() const { return mObject; }

    void updateValues();

signals:
    void bindDialogRequested(PropertyTracker* pPropertyTracker);

private:
    QObject* mObject;
    QList<PropertyWidget*> mPropertyWidgets;
};

#endif // INSPECTORPAGE_H

#ifndef INSPECTORPAGE_H
#define INSPECTORPAGE_H

#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>

#include "propertytracker.h"

class PropertyInspectorItem;

class InspectorPage : public QScrollArea
{
    Q_OBJECT

public:
    InspectorPage(QObject* pObjectToInspect);

    inline QObject* object() const { return mObject; }
    inline QGridLayout* propertiesLayout() const { return mPropertiesLayout; }

    void updateValues();

signals:
    void bindDialogRequested(PropertyTracker* pPropertyTracker);

private:
    QObject* mObject;
    QGridLayout* mPropertiesLayout;
    QList<PropertyInspectorItem*> mPropertyItems;
};

#endif // INSPECTORPAGE_H

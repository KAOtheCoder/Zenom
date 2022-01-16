#ifndef INSPECTORPAGE_H
#define INSPECTORPAGE_H

#include <QScrollArea>
#include <QScrollBar>

#include "propertywidget.h"
#include "../widget/unboundedscrollarea.h"

class InspectorPage : public UnboundedScrollArea
{
    Q_OBJECT

public:
    InspectorPage(QObject* pObjectToInspect, QWidget* pParent = nullptr);

    inline QObject* object() const { return mObject; }

    void updateValues();

signals:
    void bindDialogRequested(PropertyTracker* pPropertyTracker);

private:
    QObject* mObject;
    QList<PropertyWidget*> mPropertyWidgets;
};

#endif // INSPECTORPAGE_H

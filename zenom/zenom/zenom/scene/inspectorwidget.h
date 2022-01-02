#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <QScrollArea>
#include <QStackedWidget>

#include "propertytracker.h"

class InspectorWidget : public QStackedWidget
{
    Q_OBJECT

public:
    InspectorWidget(QWidget* pParent = nullptr);

    void setCurrentObject(QObject* pObject);
    void clear();
    void updateValues();
    void setSceneName(const QString& pName = QString());

signals:
    void bindDialogRequested(PropertyTracker* pPropertyTracker);

private:
    QHash<QObject*, int> mHash;
};

#endif // INSPECTORWIDGET_H

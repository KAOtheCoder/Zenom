#ifndef HIERARCHYIDGET_H
#define HIERARCHYWIDGET_H

#include <QTreeWidget>

#include "objecthierarchytreewidgetitem.h"

class HierarchyWidget : public QTreeWidget
{
    Q_OBJECT

public:
    HierarchyWidget(QWidget* pParent = NULL);

    void setSceneName(const QString& pName = QString());

public slots:
    void setSceneRoot(QObject* pRootObject);

private:
    ObjectHierarchyTreeWidgetItem* mSceneRootItem;
};

#endif // HIERARCHYWIDGET_H

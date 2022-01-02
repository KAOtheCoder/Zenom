#ifndef OBJECTHIERARCHYTREEWIDGETITEM_H
#define OBJECTHIERARCHYTREEWIDGETITEM_H

#include <QTreeWidgetItem>

class ObjectHierarchyTreeWidgetItem : public QTreeWidgetItem
{
public:
    ObjectHierarchyTreeWidgetItem(QObject* pObject, QTreeWidgetItem* pParent);

    inline QObject* object() const { return mObject; }

private:
    QObject* mObject;
};

#endif // OBJECTHIERARCHYTREEWIDGETITEM_H

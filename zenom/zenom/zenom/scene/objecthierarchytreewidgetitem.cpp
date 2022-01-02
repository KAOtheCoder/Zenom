#include "objecthierarchytreewidgetitem.h"

#include <QMetaProperty>
#include <QObject>

ObjectHierarchyTreeWidgetItem::ObjectHierarchyTreeWidgetItem(QObject* pObject, QTreeWidgetItem* pParent)
    : QTreeWidgetItem(pParent),
      mObject(pObject)
{
    const auto metaObject = pObject->metaObject();

    setText(0, metaObject->className());
    setText(1, pObject->objectName());
    setForeground(0, QBrush(Qt::darkMagenta));
    setForeground(1, QBrush(Qt::darkGreen));

    const auto& children = pObject->children();

    for (const auto child : children)
        new ObjectHierarchyTreeWidgetItem(child, this);

    QObject::connect(pObject, &QObject::objectNameChanged, [&](const QString& pName) { setText(1, pName); });
}

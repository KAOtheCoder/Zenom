#include "hierarchywidget.h"

HierarchyWidget::HierarchyWidget(QWidget* pParent)
    : QTreeWidget(pParent),
      mSceneRootItem(NULL)
{
    setSceneName();

    const int NUM_COLUMNS = 2;

    QStringList headerLabels;
    headerLabels.reserve(NUM_COLUMNS);
    headerLabels.push_back(tr("Type"));
    headerLabels.push_back(tr("Name"));
    setHeaderLabels(headerLabels);
}

void HierarchyWidget::setSceneRoot(QObject* pRootObject)
{
    clear();

    if (!pRootObject)
        return;

    mSceneRootItem = new ObjectHierarchyTreeWidgetItem(pRootObject, invisibleRootItem());
}

void HierarchyWidget::setSceneName(const QString& pName)
{
    static const QString TITLE_PREFIX(tr("Hierarchy: "));
    setWindowTitle(TITLE_PREFIX + pName);
}

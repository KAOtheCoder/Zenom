#include "inspectorwidget.h"
#include "inspectorpage.h"

InspectorWidget::InspectorWidget(QWidget* pParent)
    : QStackedWidget(pParent)
{
    setSceneName();
}

void InspectorWidget::setCurrentObject(QObject *pObject)
{
    if (mHash.contains(pObject))
    {
        setCurrentIndex(mHash[pObject]);
    }
    else
    {
        auto inspectorStack = new InspectorPage(pObject);
        const int widgetIndex = addWidget(inspectorStack);
        mHash[pObject] = widgetIndex;
        setCurrentIndex(widgetIndex);
        connect(inspectorStack, &InspectorPage::bindDialogRequested, this, &InspectorWidget::bindDialogRequested);
    }
}

void InspectorWidget::clear()
{
    mHash.clear();

    for(int i = count() - 1; i >= 0; --i)
    {
        auto thisWidget = widget(i);
        removeWidget(thisWidget);
        thisWidget->deleteLater();
    }
}

void InspectorWidget::updateValues()
{
    auto widget = currentWidget();

    if (widget)
        static_cast<InspectorPage*>(widget)->updateValues();
}

void InspectorWidget::setSceneName(const QString& pName)
{
    setWindowTitle(tr("Inspector: ") + pName);
}

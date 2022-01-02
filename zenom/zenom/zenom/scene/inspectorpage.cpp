#include "inspectorpage.h"
#include "propertyinspectoritem.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

InspectorPage::InspectorPage(QObject* pObjectToInspect)
    : QScrollArea(),
      mObject(pObjectToInspect),
      mPropertiesLayout(new QGridLayout())
{
    const auto metaObject = pObjectToInspect->metaObject();

    auto mainLayout = new QVBoxLayout();

    auto nameLayout = new QHBoxLayout();

    auto typeLabel = new QLabel(metaObject->className());
    auto typePalette = typeLabel->palette();
    typePalette.setColor(typeLabel->foregroundRole(), QColor(Qt::darkMagenta));
    typeLabel->setPalette(typePalette);
    nameLayout->addWidget(typeLabel);

    auto nameLabel = new QLabel(pObjectToInspect->objectName());
    auto namePalette = nameLabel->palette();
    namePalette.setColor(nameLabel->foregroundRole(), QColor(Qt::darkGreen));
    nameLabel->setPalette(namePalette);
    nameLayout->addWidget(nameLabel);
    nameLayout->addStretch();

    mainLayout->addLayout(nameLayout);

    const int propertyCount = metaObject->propertyCount();

    for (int i = 0; i < propertyCount; ++i)
    {
        const auto property = metaObject->property(i);

        if (PropertyTracker::isPropertySupported(property)) {
            auto propertyItem = new PropertyInspectorItem(this, property);
            mPropertyItems.push_back(propertyItem);
            connect(propertyItem, &PropertyInspectorItem::bindDialogRequested, this, &InspectorPage::bindDialogRequested);
        }
    }

    mainLayout->addLayout(mPropertiesLayout);
    mainLayout->addStretch();

    auto widget = new QWidget();
    widget->setLayout(mainLayout);
    setWidget(widget);

    connect(pObjectToInspect, &QObject::objectNameChanged, nameLabel, &QLabel::setText);
}

void InspectorPage::updateValues()
{
    for (auto propertyItem : mPropertyItems)
        propertyItem->updateValue();
}

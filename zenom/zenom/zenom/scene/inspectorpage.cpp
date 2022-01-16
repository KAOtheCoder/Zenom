#include "inspectorpage.h"

#include <QDebug>
#include <QLabel>

InspectorPage::InspectorPage(QObject* pObjectToInspect, QWidget* pParent)
    : UnboundedScrollArea(pParent),
      mObject(pObjectToInspect)
{
    const auto metaObject = pObjectToInspect->metaObject();

    auto mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    const int spacing = 5;
    auto nameLayout = new QHBoxLayout();
    nameLayout->setAlignment(Qt::AlignLeft);
    nameLayout->setSpacing(spacing);
    nameLayout->setContentsMargins(spacing, spacing, spacing, spacing);

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

    mainLayout->addLayout(nameLayout);

    const int propertyCount = metaObject->propertyCount();

    for (int i = 0; i < propertyCount; ++i)
    {
        auto propertyWidget = PropertyWidget::create(mObject, metaObject->property(i), this);

        if (propertyWidget) {
            QFrame* separator = new QFrame();
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            mainLayout->addWidget(separator);

            mPropertyWidgets.push_back(propertyWidget);
            mainLayout->addWidget(propertyWidget);
            connect(propertyWidget, &PropertyWidget::bindDialogRequested, this, &InspectorPage::bindDialogRequested);
        }
    }

    auto widget = new QWidget();
    widget->setLayout(mainLayout);
    setWidgetResizable(true);
    setWidget(widget);

    connect(mObject, &QObject::objectNameChanged, nameLabel, &QLabel::setText);
}

void InspectorPage::updateValues()
{
    for (auto widget : mPropertyWidgets)
        widget->updateValue();
}

#include "propertyinspectoritem.h"

PropertyInspectorItem::PropertyInspectorItem(InspectorPage* pInspector, const QMetaProperty& pProperty)
    : QObject(pInspector),
      mPropertyTracker(pInspector->object(), pProperty),
      mTypeLabel(new QLabel(LogVariableConverter::typeName(pProperty.typeId()))),
      mNameLabel(new QLabel(pProperty.name())),
      mValueLineEdit(new QLineEdit),
      mBindButton(new QPushButton(QIcon(":/zenom/resources/link.png"), QString()))
{
    auto propertiesLayout = pInspector->propertiesLayout();
    const int row = propertiesLayout->rowCount();

    auto typePalette = mTypeLabel->palette();
    typePalette.setColor(mTypeLabel->foregroundRole(), QColor(Qt::darkYellow));
    mTypeLabel->setPalette(typePalette);

    auto namePalette = mNameLabel->palette();
    namePalette.setColor(mNameLabel->foregroundRole(), QColor(Qt::darkRed));
    mNameLabel->setPalette(namePalette);

    propertiesLayout->addWidget(mTypeLabel, row, 0);
    propertiesLayout->addWidget(mNameLabel, row, 1);
    propertiesLayout->addWidget(mValueLineEdit, row, 2);
    propertiesLayout->addWidget(mBindButton, row, 3);

    connect(mValueLineEdit, &QLineEdit::editingFinished, this, &PropertyInspectorItem::setValue);
    connect(mBindButton, &QPushButton::clicked, this, [&](){ emit bindDialogRequested(&mPropertyTracker);});
}

void PropertyInspectorItem::updateValue()
{
    if (!mValueLineEdit->hasFocus())
        mValueLineEdit->setText(mPropertyTracker.valueAsString());
}

void PropertyInspectorItem::setValue()
{
    mPropertyTracker.setValueByString(mValueLineEdit->text());
}

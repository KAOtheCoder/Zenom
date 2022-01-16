#include "propertywidget.h"
#include "logvariableconverter.h"

#include <QLabel>
#include <QMetaProperty>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QColorDialog>

PropertyWidget* PropertyWidget::create(QObject* pObject, const QMetaProperty& pProperty, QWidget *pParent) {
    switch (pProperty.typeId()) {
    case QMetaType::Int:
    case QMetaType::Short:
    case QMetaType::UShort:
    case QMetaType::SChar:
    case QMetaType::UChar:
        return new IntPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::Double:
    case QMetaType::Float:
        return new DoublePropertyWidget(pObject, pProperty, pParent);
    case QMetaType::UInt:
    case QMetaType::QChar:
    case QMetaType::Long:
    case QMetaType::LongLong:
    case QMetaType::Char:
    case QMetaType::ULong:
    case QMetaType::ULongLong:
        return new PrimitivePropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QSize:
    case QMetaType::QSizeF:
        return new SizePropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QPoint:
    case QMetaType::QPointF:
        return new PointPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QRect:
    case QMetaType::QRectF:
        return new RectPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QColor:
        return new ColorPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QVector2D:
        return new Vector2dPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QVector3D:
        return new Vector3dPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QVector4D:
        return new Vector4dPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QQuaternion:
        return new QuaternionPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QMatrix4x4:
        return new Matrix4x4PropertyWidget(pObject, pProperty, pParent);
    }

    return nullptr;
}

PropertyWidget::PropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget *parent)
    : QWidget{parent},
      mPropertyTracker(pObject, pProperty)
{
    auto typeLabel = new QLabel(LogVariableConverter::typeName(pProperty.typeId()));
    auto typePalette = typeLabel->palette();
    typePalette.setColor(typeLabel->foregroundRole(), QColor(Qt::darkYellow));
    typeLabel->setPalette(typePalette);

    auto nameLabel = new QLabel(pProperty.name());
    auto namePalette = nameLabel->palette();
    namePalette.setColor(nameLabel->foregroundRole(), QColor(Qt::darkRed));
    nameLabel->setPalette(namePalette);

    auto bindButton = new QPushButton(QIcon(":/zenom/resources/link.png"), QString());

    auto titleLayout = new QHBoxLayout();
    titleLayout->addWidget(typeLabel);
    titleLayout->addWidget(nameLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(bindButton);

    const int spacing = 5;
    auto mainLayout = new QVBoxLayout();
    mainLayout->addLayout(titleLayout);
    mainLayout->setSpacing(spacing);
    mainLayout->setContentsMargins(spacing, spacing, spacing, spacing);
    setLayout(mainLayout);

    connect(bindButton, &QPushButton::clicked, this, [&](){ emit bindDialogRequested(&mPropertyTracker);});
}

PrimitivePropertyWidget::PrimitivePropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent)
    : PropertyWidget(pObject, pProperty, pParent),
      mLineEdit(new QLineEdit())
{
    if (pProperty.typeId() == QMetaType::QChar || pProperty.typeId() == QMetaType::Char)
        mLineEdit->setMaxLength(1);

    layout()->addWidget(mLineEdit);

    connect(mLineEdit, &QLineEdit::editingFinished, this, [&]() { mPropertyTracker.setValueByString(mLineEdit->text()); });
}

void PrimitivePropertyWidget::updateValue() {
    if (!mLineEdit->hasFocus())
        mLineEdit->setText(mPropertyTracker.valueAsString());
}

IntPropertyWidget::IntPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent)
    : PropertyWidget(pObject, pProperty, pParent),
      mSpinBox(new QSpinBox())
{
    mSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    switch (pProperty.typeId()) {
    case QMetaType::Short: mSpinBox->setRange(std::numeric_limits<short>::lowest(),
                                              std::numeric_limits<short>::max());
        break;
    case QMetaType::UShort: mSpinBox->setRange(std::numeric_limits<unsigned short>::lowest(),
                                               std::numeric_limits<unsigned short>::max());
        break;
    case QMetaType::SChar: mSpinBox->setRange(std::numeric_limits<signed char>::lowest(),
                                              std::numeric_limits<signed char>::max());
       break;
    case QMetaType::UChar: mSpinBox->setRange(std::numeric_limits<unsigned char>::lowest(),
                                              std::numeric_limits<unsigned char>::max());
       break;
    default: mSpinBox->setRange(std::numeric_limits<int>::lowest(),
                                std::numeric_limits<int>::max());
    }

    layout()->addWidget(mSpinBox);
    connect(mSpinBox, &QSpinBox::editingFinished, this, [&]() { mPropertyTracker.setValue(mSpinBox->value()); });
}

void IntPropertyWidget::updateValue() {
    if (!mSpinBox->hasFocus())
        mSpinBox->setValue(mPropertyTracker.value().toInt());
}

DoublePropertyWidget::DoublePropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent)
    : PropertyWidget(pObject, pProperty, pParent),
      mSpinBox(new QDoubleSpinBox())
{
    mSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    switch (pProperty.typeId()) {
    case QMetaType::Float: mSpinBox->setRange(std::numeric_limits<float>::lowest(),
                                              std::numeric_limits<float>::max());
        break;
    default: mSpinBox->setRange(std::numeric_limits<double>::lowest(),
                                std::numeric_limits<double>::max());
    }

    layout()->addWidget(mSpinBox);
    connect(mSpinBox, &QDoubleSpinBox::editingFinished, this, [&]() { mPropertyTracker.setValue(mSpinBox->value()); });
}

void DoublePropertyWidget::updateValue() {
    if (!mSpinBox->hasFocus())
        mSpinBox->setValue(mPropertyTracker.value().toDouble());
}

GridPropertyWidget::GridPropertyWidget(QObject* pObject,
                                       const QMetaProperty& pProperty,
                                       const QList<QList<QString>>& pLabels,
                                       QWidget* pParent)
    : PropertyWidget(pObject, pProperty, pParent),
      mGridLayout(new QGridLayout())
{
    const int rows = pLabels.size();
    for (int i = 0; i < rows; ++i) {
        const int columns = pLabels[i].size();

        for (int j = 0; j < columns; ++j) {
            mGridLayout->addWidget(new QLabel(pLabels[i][j]), i, 2 * j);
            auto box = new QDoubleSpinBox();
            box->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
            box->setSizePolicy(QSizePolicy::Expanding, box->sizePolicy().verticalPolicy());
            box->setButtonSymbols(QAbstractSpinBox::NoButtons);
            mGridLayout->addWidget(box, i, (2 * j) + 1);
            connect(box, &QAbstractSpinBox::editingFinished, this, &GridPropertyWidget::onEditingFinished);
        }
    }

    static_cast<QBoxLayout*>(layout())->addLayout(mGridLayout);
}

void SizePropertyWidget::updateValue() {
    const auto& size = mPropertyTracker.value().toSizeF();

    auto box = spinBox(0, 0);
    if (!box->hasFocus())
        box->setValue(size.width());

    box = spinBox(0, 1);
    if (!box->hasFocus())
        box->setValue(size.height());
}

void PointPropertyWidget::updateValue() {
    const auto& point = mPropertyTracker.value().toPointF();

    auto box = spinBox(0, 0);
    if (!box->hasFocus())
        box->setValue(point.x());

    box = spinBox(0, 1);
    if (!box->hasFocus())
        box->setValue(point.y());
}

void RectPropertyWidget::updateValue() {
    const auto& rect = mPropertyTracker.value().toRectF();

    auto box = spinBox(0, 0);
    if (!box->hasFocus())
        box->setValue(rect.x());

    box = spinBox(0, 1);
    if (!box->hasFocus())
        box->setValue(rect.y());

    box = spinBox(1, 0);
    if (!box->hasFocus())
        box->setValue(rect.width());

    box = spinBox(1, 1);
    if (!box->hasFocus())
        box->setValue(rect.height());
}

void Vector2dPropertyWidget::updateValue() {
    const auto& vector = mPropertyTracker.value().value<QVector2D>();

    for (int i = 0; i < 2; ++i) {
        auto box = spinBox(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }
}

void Vector3dPropertyWidget::updateValue() {
    const auto& vector = mPropertyTracker.value().value<QVector3D>();

    for (int i = 0; i < 3; ++i) {
        auto box = spinBox(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }
}

void Vector4dPropertyWidget::updateValue() {
    const auto& vector = mPropertyTracker.value().value<QVector4D>();

    for (int i = 0; i < 4; ++i) {
        auto box = spinBox(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }
}

void QuaternionPropertyWidget::updateValue() {
    const auto& quaternion = mPropertyTracker.value().value<QQuaternion>();
    const auto& vector = quaternion.vector();

    for (int i = 0; i < 3; ++i) {
        auto box = spinBox(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }

    auto box = spinBox(0, 3);
    if (!box->hasFocus())
        box->setValue(quaternion.scalar());
}

void Matrix4x4PropertyWidget::updateValue() {
    const auto& matrix = mPropertyTracker.value().value<QMatrix4x4>();

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            auto box = spinBox(i, j);
            if (!box->hasFocus())
                box->setValue(matrix(i, j));
        }
}

void Matrix4x4PropertyWidget::onEditingFinished() {
    float matrix[16];
    for (int i = 0; i < 16; ++i)
        matrix[i] = spinBox(i / 4, i % 4)->value();

    mPropertyTracker.setValue(QMatrix4x4(matrix));
}

ColorPropertyWidget::ColorPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent)
    : PropertyWidget(pObject, pProperty, pParent),
      mButton(new QPushButton())
{
    layout()->addWidget(mButton);

    connect(mButton, &QPushButton::clicked, [&]() {
        const auto& color = QColorDialog::getColor(mButton->palette().color(mButton->backgroundRole()), this);
        mPropertyTracker.setValue(color);
        updateValue();
    });
}

void ColorPropertyWidget::updateValue() {
    const auto& color = mPropertyTracker.value().value<QColor>();
    auto palette = mButton->palette();
    palette.setColor(mButton->backgroundRole(), color);
    mButton->setPalette(palette);
    mButton->setText(color.name(QColor::HexArgb));
}

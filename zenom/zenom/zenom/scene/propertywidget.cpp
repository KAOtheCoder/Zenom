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
    case QMetaType::Int: return new NumericPropertyWidget<int>(pObject, pProperty, pParent);
    case QMetaType::Short: return new NumericPropertyWidget<short>(pObject, pProperty, pParent);
    case QMetaType::UShort: return new NumericPropertyWidget<unsigned short>(pObject, pProperty, pParent);
    case QMetaType::SChar: return new NumericPropertyWidget<signed char>(pObject, pProperty, pParent);
    case QMetaType::UChar: return new NumericPropertyWidget<unsigned char>(pObject, pProperty, pParent);
    case QMetaType::Double: return new NumericPropertyWidget<double>(pObject, pProperty, pParent);
    case QMetaType::Float: return new NumericPropertyWidget<float>(pObject, pProperty, pParent);
    case QMetaType::UInt: return new NumericPropertyWidget<unsigned int>(pObject, pProperty, pParent);
    case QMetaType::Long: return new NumericPropertyWidget<long>(pObject, pProperty, pParent);
    case QMetaType::LongLong: return new NumericPropertyWidget<long long>(pObject, pProperty, pParent);
    case QMetaType::ULong: return new NumericPropertyWidget<unsigned long>(pObject, pProperty, pParent);
    case QMetaType::ULongLong: return new NumericPropertyWidget<unsigned long long>(pObject, pProperty, pParent);
    case QMetaType::QChar:
    case QMetaType::Char:
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
    case QMetaType::QColor: return new ColorPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QVector2D: return new Vector2dPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QVector3D: return new Vector3dPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QVector4D: return new Vector4dPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QQuaternion: return new QuaternionPropertyWidget(pObject, pProperty, pParent);
    case QMetaType::QMatrix4x4: return new Matrix4x4PropertyWidget(pObject, pProperty, pParent);
    }

    return nullptr;
}

QString PropertyWidget::typeName(const int pTypeId) {
    QString name(QMetaType(pTypeId).name());

    switch (pTypeId) {
    case QMetaType::QSizeF:
    case QMetaType::QPointF:
    case QMetaType::QRectF:
        return name.mid(1, name.size() - 2).toLower();
    case QMetaType::QSize:
    case QMetaType::QPoint:
    case QMetaType::QRect:
    case QMetaType::QColor:
    case QMetaType::QVector2D:
    case QMetaType::QVector3D:
    case QMetaType::QVector4D:
    case QMetaType::QQuaternion:
    case QMetaType::QMatrix4x4:
    case QMetaType::QUrl:
    case QMetaType::QDate:
    case QMetaType::QFont:
        return name.mid(1).toLower();
    }

    return QMetaType(pTypeId).name();
}

PropertyWidget::PropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget *parent)
    : QWidget{parent},
      mPropertyTracker(pObject, pProperty)
{
    auto typeLabel = new QLabel(PropertyWidget::typeName(pProperty.typeId()));
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

    connect(mLineEdit, &QLineEdit::editingFinished, this, [&]() { mPropertyTracker.setValue(mLineEdit->text()); });
}

void PrimitivePropertyWidget::updateValue() {
    if (!mLineEdit->hasFocus())
        mLineEdit->setText(mPropertyTracker.value().toString());
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
            auto lineEdit = new NumericLineEdit<float>();
            mGridLayout->addWidget(lineEdit, i, (2 * j) + 1);
            connect(lineEdit, &QLineEdit::editingFinished, this, &GridPropertyWidget::onEditingFinished);
        }
    }

    static_cast<QBoxLayout*>(layout())->addLayout(mGridLayout);
}

void SizePropertyWidget::updateValue() {
    const auto& size = mPropertyTracker.value().toSizeF();

    auto box = lineEdit(0, 0);
    if (!box->hasFocus())
        box->setValue(size.width());

    box = lineEdit(0, 1);
    if (!box->hasFocus())
        box->setValue(size.height());
}

void PointPropertyWidget::updateValue() {
    const auto& point = mPropertyTracker.value().toPointF();

    auto box = lineEdit(0, 0);
    if (!box->hasFocus())
        box->setValue(point.x());

    box = lineEdit(0, 1);
    if (!box->hasFocus())
        box->setValue(point.y());
}

void RectPropertyWidget::updateValue() {
    const auto& rect = mPropertyTracker.value().toRectF();

    auto box = lineEdit(0, 0);
    if (!box->hasFocus())
        box->setValue(rect.x());

    box = lineEdit(0, 1);
    if (!box->hasFocus())
        box->setValue(rect.y());

    box = lineEdit(1, 0);
    if (!box->hasFocus())
        box->setValue(rect.width());

    box = lineEdit(1, 1);
    if (!box->hasFocus())
        box->setValue(rect.height());
}

void Vector2dPropertyWidget::updateValue() {
    const auto& vector = mPropertyTracker.value().value<QVector2D>();

    for (int i = 0; i < 2; ++i) {
        auto box = lineEdit(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }
}

void Vector3dPropertyWidget::updateValue() {
    const auto& vector = mPropertyTracker.value().value<QVector3D>();

    for (int i = 0; i < 3; ++i) {
        auto box = lineEdit(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }
}

void Vector4dPropertyWidget::updateValue() {
    const auto& vector = mPropertyTracker.value().value<QVector4D>();

    for (int i = 0; i < 4; ++i) {
        auto box = lineEdit(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }
}

void QuaternionPropertyWidget::updateValue() {
    const auto& quaternion = mPropertyTracker.value().value<QQuaternion>();
    const auto& vector = quaternion.vector();

    for (int i = 0; i < 3; ++i) {
        auto box = lineEdit(0, i);
        if (!box->hasFocus())
            box->setValue(vector[i]);
    }

    auto box = lineEdit(0, 3);
    if (!box->hasFocus())
        box->setValue(quaternion.scalar());
}

void Matrix4x4PropertyWidget::updateValue() {
    const auto& matrix = mPropertyTracker.value().value<QMatrix4x4>();

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            auto box = lineEdit(i, j);
            if (!box->hasFocus())
                box->setValue(matrix(i, j));
        }
}

void Matrix4x4PropertyWidget::onEditingFinished() {
    float matrix[16];
    for (int i = 0; i < 16; ++i)
        matrix[i] = lineEdit(i / 4, i % 4)->value();

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

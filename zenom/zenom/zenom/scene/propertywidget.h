#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QDoubleSpinBox>

#include "propertytracker.h"
#include "numericlineedit.h"

class PropertyWidget : public QWidget
{
    Q_OBJECT

public:
    static PropertyWidget* create(QObject* pObject, const QMetaProperty& pProperty, QWidget *pParent = nullptr);

    explicit PropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget *pParent = nullptr);

    virtual void updateValue() = 0;

signals:
    void bindDialogRequested(PropertyTracker*);

protected:
    PropertyTracker mPropertyTracker;

private:
    static QString typeName(const int pTypeId);
};

class PrimitivePropertyWidget : public PropertyWidget {
    Q_OBJECT

public:
    PrimitivePropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr);

    void updateValue() override;

private:
    QLineEdit* mLineEdit;
};

template <typename T>
class NumericPropertyWidget: public PropertyWidget {
public:
    NumericPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : PropertyWidget(pObject, pProperty, pParent),
          mLineEdit(new NumericLineEdit<T>())
    {
        layout()->addWidget(mLineEdit);

        connect(mLineEdit, &QLineEdit::editingFinished, this, [&]() { mPropertyTracker.setValue(mLineEdit->text()); });
    }

    void updateValue() override {
        if (!mLineEdit->hasFocus())
            mLineEdit->setText(mPropertyTracker.value().toString());
    }

private:
    NumericLineEdit<T>* mLineEdit;
};

class GridPropertyWidget : public PropertyWidget {
    Q_OBJECT

public:
    GridPropertyWidget(QObject* pObject,
                       const QMetaProperty& pProperty,
                       const QList<QList<QString>>& pLabels,
                       QWidget* pParent = nullptr);

    NumericLineEdit<float>* lineEdit(const int pRow, const int pColumn) const {
        return static_cast<NumericLineEdit<float>*>(mGridLayout->itemAtPosition(pRow, (pColumn * 2) + 1)->widget());
    }

protected:
    virtual void onEditingFinished() = 0;

private:
    QGridLayout* mGridLayout;
};

class SizePropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    SizePropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject, pProperty, {{tr("width"), tr("height")}}, pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override {
        mPropertyTracker.setValue(QSizeF(lineEdit(0, 0)->value(),
                                         lineEdit(0, 1)->value()));
    }
};

class PointPropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    PointPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject, pProperty, {{"x", "y"}}, pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override {
        mPropertyTracker.setValue(QPointF(lineEdit(0, 0)->value(),
                                          lineEdit(0, 1)->value()));
    }
};

class RectPropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    RectPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject, pProperty, {{"x", "y"}, {tr("width"), tr("height")}}, pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override {
        mPropertyTracker.setValue(QRectF(lineEdit(0, 0)->value(),
                                          lineEdit(0, 1)->value(),
                                          lineEdit(1, 0)->value(),
                                          lineEdit(1, 1)->value()));
    }
};

class Vector2dPropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    Vector2dPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject, pProperty, {{"x", "y"}}, pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override {
        mPropertyTracker.setValue(QVector2D(lineEdit(0, 0)->value(),
                                            lineEdit(0, 1)->value()));
    }
};

class Vector3dPropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    Vector3dPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject, pProperty, {{"x", "y", "z"}}, pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override {
        mPropertyTracker.setValue(QVector3D(lineEdit(0, 0)->value(),
                                            lineEdit(0, 1)->value(),
                                            lineEdit(0, 2)->value()));
    }
};

class Vector4dPropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    Vector4dPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject, pProperty, {{"x", "y", "z", "w"}}, pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override {
        mPropertyTracker.setValue(QVector4D(lineEdit(0, 0)->value(),
                                            lineEdit(0, 1)->value(),
                                            lineEdit(0, 2)->value(),
                                            lineEdit(0, 3)->value()));
    }
};

class QuaternionPropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    QuaternionPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject, pProperty, {{"x", "y", "z", tr("scalar")}}, pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override {
        mPropertyTracker.setValue(QQuaternion(lineEdit(0, 3)->value(),
                                              lineEdit(0, 0)->value(),
                                              lineEdit(0, 1)->value(),
                                              lineEdit(0, 2)->value()));
    }
};

class Matrix4x4PropertyWidget : public GridPropertyWidget {
    Q_OBJECT

public:
    Matrix4x4PropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr)
        : GridPropertyWidget(pObject,
                             pProperty,
                             {{"m11", "m12", "m13", "m14"},
                              {"m21", "m22", "m23", "m24"},
                              {"m31", "m32", "m33", "m34"},
                              {"m41", "m42", "m43", "m44"}},
                             pParent)
    {}

    void updateValue() override;

protected:
    void onEditingFinished() override;
};

class ColorPropertyWidget : public PropertyWidget {
    Q_OBJECT

public:
    ColorPropertyWidget(QObject* pObject, const QMetaProperty& pProperty, QWidget* pParent = nullptr);

    void updateValue() override;

private:
    QPushButton* mButton;
};

#endif // PROPERTYWIDGET_H

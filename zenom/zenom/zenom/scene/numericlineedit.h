#ifndef NUMERICLINEEDIT_H
#define NUMERICLINEEDIT_H

#include <QLineEdit>
#include <QValidator>

template <typename T>
class NumericValidator : public QValidator {
public:
    NumericValidator(QWidget* pParent = nullptr)
        : QValidator(pParent)
    {}

    void fixup(QString& input) const override {
        Q_UNUSED(input);
    }

    QValidator::State validate(QString& input, int& pos) const override {
        Q_UNUSED(pos);

        const QVariant variant(input);
        const T value = variant.value<T>();
        if (!value) {
            bool ok;
            variant.toDouble(&ok);

            if (!ok)
                return QValidator::Invalid;
        }

        return QValidator::Acceptable;
    }
};

template <typename T>
class NumericLineEdit : public QLineEdit
{
public:
    NumericLineEdit(QWidget* pParent = nullptr)
        : QLineEdit(pParent)
    {
        setValidator(new NumericValidator<T>(this));

        connect(this, &QLineEdit::cursorPositionChanged, [&]() {
            if (!hasFocus())
                home(false);
        });
    }

    QSize sizeHint() const override { return QSize(minimumWidth(), QLineEdit::sizeHint().height()); }

    QSize minimumSizeHint() const override {
        QFontMetrics fm(font());
        return QSize(fm.horizontalAdvance('0') * 5, QLineEdit::minimumSizeHint().height());
    }

    T value() const { return QVariant(text()).value<T>(); }
    void setValue(const T value) { setText(QVariant(value).toString()); }
};

#endif // NUMERICLINEEDIT_H

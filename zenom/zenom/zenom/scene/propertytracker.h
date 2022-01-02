#ifndef PROPERTYTRACKER_H
#define PROPERTYTRACKER_H

#include <QMetaProperty>

#include "logvariableconverter.h"

class PropertyTracker
{
public:
    static bool isPropertySupported(const QMetaProperty& property);

    PropertyTracker(QObject* pObject, const QMetaProperty& pProperty);

    inline QMetaProperty property() const { return mProperty; }

    bool canBind(LogVariable* pLogVariable) const;
    bool isBinded() const;
    void bind(LogVariable* pLogVariable);
    void unbind();

    bool canUpdate() const;
    void update();
    QString valueAsString() const;
    void setValueByString(const QString& pValue);

private:
    QObject* mObject;
    QMetaProperty mProperty;
    LogVariableConverter mLogVariableConverter;
};

#endif // PROPERTYTRACKER_H

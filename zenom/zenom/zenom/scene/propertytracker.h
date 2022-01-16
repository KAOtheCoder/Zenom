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

    QVariant value() const { return mObject->property(mProperty.name()); }
    void setValue(const QVariant& pValue) { mObject->setProperty(mProperty.name(), pValue); }

private:
    QObject* mObject;
    QMetaProperty mProperty;
    LogVariableConverter mLogVariableConverter;
};

#endif // PROPERTYTRACKER_H

#include "propertytracker.h"

bool PropertyTracker::isPropertySupported(const QMetaProperty &property)
{
    return !property.isEnumType() && QMetaType::canConvert(QMetaType::fromType<LogVariableConverter>(), QMetaType(property.typeId()));
}

PropertyTracker::PropertyTracker(QObject* pObject, const QMetaProperty& pProperty)
    : mObject(pObject),
      mProperty(pProperty)
{
}

bool PropertyTracker::canBind(LogVariable *pLogVariable) const
{
    LogVariableConverter converter(pLogVariable);
    return converter.canConvert(static_cast<QMetaType::Type>(mProperty.typeId()));
}

bool PropertyTracker::isBinded() const
{
    return mLogVariableConverter.logVariable() != nullptr;
}

void PropertyTracker::bind(LogVariable * pLogVariable)
{
    mLogVariableConverter.setLogVariable(pLogVariable);
}

void PropertyTracker::unbind()
{
    mLogVariableConverter.setLogVariable(nullptr);
}

bool PropertyTracker::canUpdate() const
{
    return !mLogVariableConverter.isNull() && mLogVariableConverter.logVariable()->isHeapValid();
}

void PropertyTracker::update()
{
    mObject->setProperty(mProperty.name(), QVariant::fromValue(mLogVariableConverter));
}

QString PropertyTracker::valueAsString() const
{
    return mObject->property(mProperty.name()).toString();
}

void PropertyTracker::setValueByString(const QString &pValue)
{
    mObject->setProperty(mProperty.name(), QVariant(pValue));
}

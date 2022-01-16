#include "propertytracker.h"

PropertyTracker::PropertyTracker(QObject* pObject, const QMetaProperty& pProperty)
    : mObject(pObject),
      mProperty(pProperty)
{}

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

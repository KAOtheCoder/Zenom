#include "logvariableconverter.h"

LogVariableConverter::LogVariableConverter(LogVariable* pLogVariable)
    : mLogVariable(pLogVariable)
{}

bool LogVariableConverter::canConvert(const QMetaType::Type pType) const
{
    if (!isNull() && QMetaType::canConvert(QMetaType::fromType<LogVariableConverter>(), QMetaType(pType))) {
        switch (pType)
        {
        case QMetaType::QSize:
        case QMetaType::QSizeF:
        case QMetaType::QPoint:
        case QMetaType::QPointF:
            return mLogVariable->row() == 1 && mLogVariable->col() == 2;

        case QMetaType::QVector2D:
            return mLogVariable->row() == 1 && mLogVariable->col() >= 2;

        case QMetaType::QVector3D:
        case QMetaType::QColor:
            return mLogVariable->row() == 1 && mLogVariable->col() >= 3;

        case QMetaType::QVector4D:
        case QMetaType::QQuaternion:
            return mLogVariable->row() == 1 && mLogVariable->col() == 4;

        case QMetaType::QRect:
        case QMetaType::QRectF:
            return mLogVariable->row() == 2 && mLogVariable->col() == 2;

        case QMetaType::QMatrix4x4:
            return mLogVariable->row() == 4 && mLogVariable->col() == 4;

        default:
            return mLogVariable->row() == 1 && mLogVariable->col() == 1;
        }
    }

    return false;
}

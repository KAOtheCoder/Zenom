#include "logvariableconverter.h"

LogVariableConverter::LogVariableConverter(LogVariable* pLogVariable)
    : mLogVariable(pLogVariable)
{}

QString LogVariableConverter::typeName(const int pTypeId) {
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

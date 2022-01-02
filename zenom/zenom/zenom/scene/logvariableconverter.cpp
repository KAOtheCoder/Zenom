#include "logvariableconverter.h"

#include <QSize>
#include <QSizeF>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QRect>
#include <QRectF>
#include <QPoint>
#include <QMatrix4x4>

LogVariableConverter::LogVariableConverter(LogVariable* pLogVariable)
    : mLogVariable(pLogVariable)
{}

QString LogVariableConverter::typeName(int pTypeId) {
    switch (pTypeId) {
    case QMetaType::QSize:
    case QMetaType::QSizeF:
        return "size";
    case QMetaType::QPoint:
    case QMetaType::QPointF:
        return "point";
    case QMetaType::QRect:
    case QMetaType::QRectF:
        return "rect";
    case QMetaType::QColor:
        return "color";
    case QMetaType::QVector2D:
        return "vector2d";
    case QMetaType::QVector3D:
        return "vector3d";
    case QMetaType::QVector4D:
        return "vector4d";
    case QMetaType::QQuaternion:
        return "quaternion";
    case QMetaType::QMatrix4x4:
        return "matrix4x4";
    case QMetaType::QUrl:
        return "url";
    case QMetaType::QDate:
        return "date";
    case QMetaType::QFont:
        return "font";
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

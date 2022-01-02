#ifndef LOGVARIABLECONVERTER_H
#define LOGVARIABLECONVERTER_H

#include "logvariable.h"

#include <QVariant>
#include <functional>
#include <QPointF>
#include <QQuaternion>
#include <QColor>
#include <QMatrix4x4>

class LogVariableConverter
{
public:
    static QString typeName(const int pTypeId);

    LogVariableConverter(LogVariable* pLogVariable = NULL);

    inline LogVariable* logVariable() const { return mLogVariable; }
    inline void setLogVariable(LogVariable* pLogVariable) { mLogVariable = pLogVariable; }
    inline bool isNull() const { return !mLogVariable; }
    bool canConvert(const QMetaType::Type pType) const;

    operator double() const { return mLogVariable->lastHeapElement(0); }
    operator int() const { return mLogVariable->lastHeapElement(0); }
    operator float() const { return mLogVariable->lastHeapElement(0); }
    operator QColor() const { return QColor::fromRgbF(mLogVariable->lastHeapElement(0),
                                                      mLogVariable->lastHeapElement(1),
                                                      mLogVariable->lastHeapElement(2),
                                                      mLogVariable->col() >= 4 ? mLogVariable->lastHeapElement(3) : 1); }
    operator QMatrix4x4() const { return QMatrix4x4(mLogVariable->lastHeapElement(0, 0),
                                                    mLogVariable->lastHeapElement(0, 1),
                                                    mLogVariable->lastHeapElement(0, 2),
                                                    mLogVariable->lastHeapElement(0, 3),
                                                    mLogVariable->lastHeapElement(1, 0),
                                                    mLogVariable->lastHeapElement(1, 1),
                                                    mLogVariable->lastHeapElement(1, 2),
                                                    mLogVariable->lastHeapElement(1, 3),
                                                    mLogVariable->lastHeapElement(2, 0),
                                                    mLogVariable->lastHeapElement(2, 1),
                                                    mLogVariable->lastHeapElement(2, 2),
                                                    mLogVariable->lastHeapElement(2, 3),
                                                    mLogVariable->lastHeapElement(3, 0),
                                                    mLogVariable->lastHeapElement(3, 1),
                                                    mLogVariable->lastHeapElement(3, 2),
                                                    mLogVariable->lastHeapElement(3, 3)); }
    operator QPointF() const { return QPointF(mLogVariable->lastHeapElement(0), mLogVariable->lastHeapElement(1)); }
    operator QPoint() const { return operator QPointF().toPoint(); }
    operator QQuaternion() const { return QQuaternion(mLogVariable->lastHeapElement(0),
                                                      mLogVariable->lastHeapElement(1),
                                                      mLogVariable->lastHeapElement(2),
                                                      mLogVariable->lastHeapElement(3)); }
    operator QRectF() const { return QRectF(mLogVariable->lastHeapElement(0, 0), mLogVariable->lastHeapElement(0, 1),
                                            mLogVariable->lastHeapElement(1, 0), mLogVariable->lastHeapElement(1, 1)); }
    operator QRect() const { return operator QRectF().toRect(); }
    operator QSizeF() const { return QSizeF(mLogVariable->lastHeapElement(0), mLogVariable->lastHeapElement(1)); }
    operator QSize() const { return operator QSizeF().toSize(); }
    operator QVector2D() const { return QVector2D(mLogVariable->lastHeapElement(0),
                                                  mLogVariable->lastHeapElement(1)); }
    operator QVector3D() const { return QVector3D(mLogVariable->lastHeapElement(0),
                                                  mLogVariable->lastHeapElement(1),
                                                  mLogVariable->col() >= 3 ? mLogVariable->lastHeapElement(2) : 0); }
    operator QVector4D() const { return QVector4D(mLogVariable->lastHeapElement(0),
                                                  mLogVariable->lastHeapElement(1),
                                                  mLogVariable->col() >= 3 ? mLogVariable->lastHeapElement(2) : 0,
                                                  mLogVariable->col() >= 4 ? mLogVariable->lastHeapElement(3) : 0); }

private:
    LogVariable* mLogVariable;
    std::function<QVariant(LogVariable*)> mConverter;
};

#endif // LOGVARIABLECONVERTER_H

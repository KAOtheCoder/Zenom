/****************************************************************************
** Meta object code from reading C++ file 'plotmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../zenom/zenom/plot/plotmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'plotmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlotManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   12,   13,   12, 0x0a,
      50,   38,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PlotManager[] = {
    "PlotManager\0\0PlotWindow*\0createPlot()\0"
    "pPlotWindow\0plotWindowDestroyed(PlotWindow*)\0"
};

void PlotManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PlotManager *_t = static_cast<PlotManager *>(_o);
        switch (_id) {
        case 0: { PlotWindow* _r = _t->createPlot();
            if (_a[0]) *reinterpret_cast< PlotWindow**>(_a[0]) = _r; }  break;
        case 1: _t->plotWindowDestroyed((*reinterpret_cast< PlotWindow*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PlotManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PlotManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PlotManager,
      qt_meta_data_PlotManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PlotManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PlotManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PlotManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlotManager))
        return static_cast<void*>(const_cast< PlotManager*>(this));
    return QObject::qt_metacast(_clname);
}

int PlotManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
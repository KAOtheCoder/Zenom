/****************************************************************************
** Meta object code from reading C++ file 'gaugemanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../zenom/zenom/gauge/gaugemanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gaugemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GaugeManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      49,   36,   13,   13, 0x08,
      82,   36,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GaugeManager[] = {
    "GaugeManager\0\0showGaugeListDialog()\0"
    "pGaugeWidget\0gaugeWidgetCreated(GaugeWidget*)\0"
    "gaugeWidgetDestroyed(GaugeWidget*)\0"
};

void GaugeManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GaugeManager *_t = static_cast<GaugeManager *>(_o);
        switch (_id) {
        case 0: _t->showGaugeListDialog(); break;
        case 1: _t->gaugeWidgetCreated((*reinterpret_cast< GaugeWidget*(*)>(_a[1]))); break;
        case 2: _t->gaugeWidgetDestroyed((*reinterpret_cast< GaugeWidget*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GaugeManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GaugeManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GaugeManager,
      qt_meta_data_GaugeManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GaugeManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GaugeManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GaugeManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GaugeManager))
        return static_cast<void*>(const_cast< GaugeManager*>(this));
    return QObject::qt_metacast(_clname);
}

int GaugeManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
/****************************************************************************
** Meta object code from reading C++ file 'NetworkTransfert.h'
**
** Created: Sun Mar 24 16:26:31 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NetworkTransfert.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkTransfert.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NetworkTransfert[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      31,   17,   17,   17, 0x08,
      46,   17,   17,   17, 0x08,
      73,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NetworkTransfert[] = {
    "NetworkTransfert\0\0sendRecord()\0"
    "readResponse()\0connectionClosedByServer()\0"
    "error()\0"
};

const QMetaObject NetworkTransfert::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NetworkTransfert,
      qt_meta_data_NetworkTransfert, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NetworkTransfert::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NetworkTransfert::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NetworkTransfert::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkTransfert))
        return static_cast<void*>(const_cast< NetworkTransfert*>(this));
    return QObject::qt_metacast(_clname);
}

int NetworkTransfert::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendRecord(); break;
        case 1: readResponse(); break;
        case 2: connectionClosedByServer(); break;
        case 3: error(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

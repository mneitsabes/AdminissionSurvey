/****************************************************************************
** Meta object code from reading C++ file 'RessourceErrorException.h'
**
** Created: Sun Mar 24 16:26:30 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "RessourceErrorException.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RessourceErrorException.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RessourceErrorException[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_RessourceErrorException[] = {
    "RessourceErrorException\0"
};

const QMetaObject RessourceErrorException::staticMetaObject = {
    { &RessourceException::staticMetaObject, qt_meta_stringdata_RessourceErrorException,
      qt_meta_data_RessourceErrorException, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RessourceErrorException::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RessourceErrorException::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RessourceErrorException::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RessourceErrorException))
        return static_cast<void*>(const_cast< RessourceErrorException*>(this));
    return RessourceException::qt_metacast(_clname);
}

int RessourceErrorException::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RessourceException::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

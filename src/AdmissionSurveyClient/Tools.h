#ifndef TOOLS_H
#define TOOLS_H

#include <QFile>
#include <QByteArray>
#include <QString>

#include "IllegalArgumentException.h"

/*
 * Boite � outils.
 */
class Tools
{
    public:
        static QByteArray readAllFile(QString path);
};

#endif // TOOLS_H

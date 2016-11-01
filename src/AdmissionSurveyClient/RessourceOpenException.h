#ifndef RESSOURCEOPENEXCEPTION_H
#define RESSOURCEOPENEXCEPTION_H

#include <QObject>
#include <QString>

#include "RessourceException.h"

/*
 * Exception lancee lorsqu'il y a un probleme d'ouverture du fichier descriptif.
 */
class RessourceOpenException : public RessourceException
{
    Q_OBJECT

    private:
        /* Le fichier qui a provoqué l'erreur d'ouverture */
        QString m_file;

    public:
        RessourceOpenException(QString file) throw();
        ~RessourceOpenException() throw();
        QString getFile();
        QString getTitle();
        QString getMessage();
};

#endif // RESSOURCEOPENEXCEPTION_H

#ifndef RESSOURCEPARSINGEXCEPTION_H
#define RESSOURCEPARSINGEXCEPTION_H

#include <QObject>

#include "RessourceException.h"

/*
 * Exception lancée lorsqu'il y a un problème pendant l'analyse du fichier par le parser de Qt (erreur de synthaxe).
 */
class RessourceParsingException : public RessourceException
{
    Q_OBJECT

    private:
        /* Le numéro de la ligne qui a provoqué l'erreur */
        int m_line;

    public:
        RessourceParsingException(int line) throw();
        ~RessourceParsingException() throw();
        int getLine();
        QString getTitle();
        QString getMessage();
};

#endif // RESSOURCEPARSINGEXCEPTION_H

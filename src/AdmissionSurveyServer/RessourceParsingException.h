#ifndef RESSOURCEPARSINGEXCEPTION_H
#define RESSOURCEPARSINGEXCEPTION_H

#include <QObject>

#include "RessourceException.h"

/*
 * Exception lanc�e lorsqu'il y a un probl�me pendant l'analyse du fichier par le parser de Qt (erreur de synthaxe).
 */
class RessourceParsingException : public RessourceException
{
    Q_OBJECT

    private:
        /* Le num�ro de la ligne qui a provoqu� l'erreur */
        int m_line;

    public:
        RessourceParsingException(int line) throw();
        ~RessourceParsingException() throw();
        int getLine();
        QString getTitle();
        QString getMessage();
};

#endif // RESSOURCEPARSINGEXCEPTION_H

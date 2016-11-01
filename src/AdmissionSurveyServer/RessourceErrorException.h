#ifndef RESSOURCEERROREXCEPTION_H
#define RESSOURCEERROREXCEPTION_H

#include <QObject>
#include <QString>

#include "RessourceException.h"

/*
 * Permet de preciser le type de ressource qui a provoque l'erreur.
 */
enum RessourceErrorType {
    /* Un fichier HTML */
    HTML_FILE,
};

/*!
 *  Exception lancee lorsqu'une ressource a pose probleme.
 *
 *  Exception utilisee lorsqu'une ressource a pose probleme lors de l'ouverture par exemple.
 */
class RessourceErrorException : public RessourceException
{
    Q_OBJECT

    private:
        /* Le type de ressource */
        RessourceErrorType m_type;

        /* L'ID de la question */
        int m_questionId;

    public:
        RessourceErrorException(RessourceErrorType type, int questionId) throw();
        ~RessourceErrorException() throw();
        RessourceErrorType getType();
        int getQuestionId();
        QString getTitle();
        QString getMessage();
};

#endif // RESSOURCEERROREXCEPTION_H

#ifndef RESPONSE_H
#define RESPONSE_H

#include <QVector>

#include "ResponseItem.h"
#include "Language.h"

/*
 * Les différents types de réponses possibles.
 */
enum ResponseType
{
    /* Réponse : "oui - non - Je ne sais pas" */
    YES_NO_UNKNOWN,

    /* Réponse : "oui - non" */
    YES_NO,

    /* Réponses déclarées dans le fichier XML */
    DECLARED,

    /* Réponses déclarées dans le fichier HTML de la question et dans le fichier XML pour les valeurs liées */
    EMBEDDED
};

/*
 * Représente un ensemble de réponses possibles pour une question.
 */
class Response
{
    private:
        /* Le type de la réponse */
        ResponseType m_type;

    protected:
        /* Les différents éléments de réponse */
        QVector<ResponseItem *> m_responseItems;

    public:
        Response(ResponseType type);
        virtual ~Response() = 0;
        ResponseType getType();
        ResponseItem * getResponseItem(QString tag);
        QVector<ResponseItem *> getAllResponseItem();
        virtual QByteArray getHTMLData(Language lg) = 0;
};

#endif // RESPONSE_H

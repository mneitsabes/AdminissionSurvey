#ifndef RESPONSE_H
#define RESPONSE_H

#include <QVector>

#include "ResponseItem.h"
#include "Language.h"

/*
 * Les diff�rents types de r�ponses possibles.
 */
enum ResponseType
{
    /* R�ponse : "oui - non - Je ne sais pas" */
    YES_NO_UNKNOWN,

    /* R�ponse : "oui - non" */
    YES_NO,

    /* R�ponses d�clar�es dans le fichier XML */
    DECLARED,

    /* R�ponses d�clar�es dans le fichier HTML de la question et dans le fichier XML pour les valeurs li�es */
    EMBEDDED
};

/*
 * Repr�sente un ensemble de r�ponses possibles pour une question.
 */
class Response
{
    private:
        /* Le type de la r�ponse */
        ResponseType m_type;

    protected:
        /* Les diff�rents �l�ments de r�ponse */
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

#ifndef RESPONSEDECLARED_H
#define RESPONSEDECLARED_H

#include <QByteArray>

#include "Tools.h"
#include "Response.h"
#include "ResponseItem.h"
#include "Language.h"
#include "IllegalArgumentException.h"

/*
 * Disposition des réponses générées à partir du fichier XML.
 */
enum ResponsesDisposition {
    /* Réponses disposées horizontalement */
    HORIZONTAL,

    /* Réponses disposées verticalement */
    VERTICAL
};

/*
 * Une réponse déclarée a ses différents éléments de réponse déclarés dans le fichier XML.
 *
 * Les réponses peuvent etre disposées de différentes manières, par défaut elles sont disposées verticalement.
 */
class ResponseDeclared : public Response
{
    private:
        ResponsesDisposition m_disposition;

    public:
        ResponseDeclared();
        ~ResponseDeclared();
        void addResponseItem(ResponseItem *ri);
        void setResponsesDisposition(ResponsesDisposition disposition);
        QByteArray getHTMLData(Language lg);

    private:
        QByteArray getHTMLDataHorizontal(QByteArray data, Language lg);
        QByteArray getHTMLDataVertical(QByteArray data, Language lg);
        int getSizeResponseItem(Language lg);
};

#endif // RESPONSEDECLARED_H

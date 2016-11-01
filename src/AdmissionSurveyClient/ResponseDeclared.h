#ifndef RESPONSEDECLARED_H
#define RESPONSEDECLARED_H

#include <QByteArray>

#include "Tools.h"
#include "Response.h"
#include "ResponseItem.h"
#include "Language.h"
#include "IllegalArgumentException.h"

/*
 * Disposition des r�ponses g�n�r�es � partir du fichier XML.
 */
enum ResponsesDisposition {
    /* R�ponses dispos�es horizontalement */
    HORIZONTAL,

    /* R�ponses dispos�es verticalement */
    VERTICAL
};

/*
 * Une r�ponse d�clar�e a ses diff�rents �l�ments de r�ponse d�clar�s dans le fichier XML.
 *
 * Les r�ponses peuvent etre dispos�es de diff�rentes mani�res, par d�faut elles sont dispos�es verticalement.
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

#ifndef RESPONSEEMBEDDED_H
#define RESPONSEEMBEDDED_H


#include <QByteArray>

#include "Response.h"
#include "ResponseItem.h"
#include "Language.h"
#include "IllegalArgumentException.h"

/*
 * Une r�ponse encapsul�e dans le fichier HTML de la question mais les �l�ments de r�ponse d�clar�s dans le fichier XML.
 */
class ResponseEmbedded : public Response
{
    public:
        ResponseEmbedded();
        ~ResponseEmbedded();
        void addResponseItem(ResponseItem *ri);
        QByteArray getHTMLData(Language lg);
};

#endif // RESPONSEEMBEDDED_H

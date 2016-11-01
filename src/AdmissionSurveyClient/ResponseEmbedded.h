#ifndef RESPONSEEMBEDDED_H
#define RESPONSEEMBEDDED_H


#include <QByteArray>

#include "Response.h"
#include "ResponseItem.h"
#include "Language.h"
#include "IllegalArgumentException.h"

/*
 * Une réponse encapsulée dans le fichier HTML de la question mais les éléments de réponse déclarés dans le fichier XML.
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

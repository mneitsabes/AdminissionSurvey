#include "ResponseEmbedded.h"

/*
 * Constructeur.
 */
ResponseEmbedded::ResponseEmbedded() : Response(EMBEDDED) { }

/*
 * Destructeur.
 */
ResponseEmbedded::~ResponseEmbedded() { }

/*
 * Permet d'ajoute un élément de réponse.
 *
 * @param ri l'élément de réponse
 */
void ResponseEmbedded::addResponseItem(ResponseItem *ri)
{
    if(ri != NULL)
        m_responseItems.append(ri);
    else
        throw new IllegalArgumentException("RI cannot be null");
}

/*
 * Pas de données à injecter.
 *
 * Implémentée pour répondre aux exigences de la classe mère.
 *
 * @param lg la langue
 * @return aucune donnée
 */
QByteArray ResponseEmbedded::getHTMLData(Language lg)
{
    QByteArray data;
    return data;
}

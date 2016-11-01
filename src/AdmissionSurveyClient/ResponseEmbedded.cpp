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
 * Permet d'ajoute un �l�ment de r�ponse.
 *
 * @param ri l'�l�ment de r�ponse
 */
void ResponseEmbedded::addResponseItem(ResponseItem *ri)
{
    if(ri != NULL)
        m_responseItems.append(ri);
    else
        throw new IllegalArgumentException("RI cannot be null");
}

/*
 * Pas de donn�es � injecter.
 *
 * Impl�ment�e pour r�pondre aux exigences de la classe m�re.
 *
 * @param lg la langue
 * @return aucune donn�e
 */
QByteArray ResponseEmbedded::getHTMLData(Language lg)
{
    QByteArray data;
    return data;
}

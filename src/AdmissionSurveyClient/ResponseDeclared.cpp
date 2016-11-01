#include "ResponseDeclared.h"

/*
 * Constructeur.
 */
ResponseDeclared::ResponseDeclared() : Response(DECLARED)
{
    m_disposition = VERTICAL;
}

/*
 * Destructeur.
 */
ResponseDeclared::~ResponseDeclared() { }

/*
 * Permet de d�finir la disposition des r�ponses.
 *
 * @param disposition la disposition
 */
void ResponseDeclared::setResponsesDisposition(ResponsesDisposition disposition)
{
    m_disposition = disposition;
}

/*
 * Permet d'ajoute un �l�ment de r�ponse.
 *
 * @param ri l'�l�ment de r�ponse
 */
void ResponseDeclared::addResponseItem(ResponseItem *ri)
{
    if(ri != NULL)
        m_responseItems.append(ri);
    else
        throw new IllegalArgumentException("RI cannot be null");
}

/*
 * Retourne les donn�es HTML de la r�ponse en fonction de la langue sp�cifi�e.
 *
 * @param lg la langue
 * @return donn�es HTML
 */
QByteArray ResponseDeclared::getHTMLData(Language lg)
{
    QByteArray data = "<br/><br/><br/><br/><center><div id=\"responses\">\n";

    switch(m_disposition)
    {
        case VERTICAL:
            data = getHTMLDataVertical(data, lg);
        break;
        case HORIZONTAL:
            data = getHTMLDataHorizontal(data, lg);
        break;
    }

    data = data.append("</div></center>");

    return data;
}

/*
 * Retourne les donn�es HTML avec les r�ponses dispos�es horizontalement.
 *
 * @param data les donn�es pr�c�dent le tableau contenant les r�ponses
 * @param lg la langue
 * @return les donn�es HTML
 */
QByteArray ResponseDeclared::getHTMLDataHorizontal(QByteArray data, Language lg)
{
    data = data.append("<table width=\"75%\">\n<tr>");

    int maxWidth = getSizeResponseItem(lg);

    for(int i=0; i < m_responseItems.size(); i++)
        data = data.append("<td align=\"center\"><a href=\"#\" class=\"button\" style=\"width:"+ QString::number(maxWidth) +"px;\" onclick=\"setSelected(this, '"+ m_responseItems.at(i)->getTag() + "');\">" + m_responseItems.at(i)->getLabel(lg) + "</a></td>");

    data = data.append("</tr>\n</table>");

    return data;
}

/*
 * Retourne les donn�es HTML avec les r�ponses dispos�es verticalement.
 *
 * @param data les donn�es pr�c�dent le tableau contenant les r�ponses
 * @param lg la langue
 * @return les donn�es HTML
 */
QByteArray ResponseDeclared::getHTMLDataVertical(QByteArray data, Language lg)
{
    data = data.append("<table>\n");

    int maxWidth = getSizeResponseItem(lg);

    for(int i=0; i < m_responseItems.size(); i++)
        data = data.append("<tr><td><a href=\"#\" class=\"button\" style=\"width:"+ QString::number(maxWidth) +"px;\" onclick=\"setSelected(this, '" + m_responseItems.at(i)->getTag() + "');\">" + m_responseItems.at(i)->getLabel(lg) + "</a></td></tr>");

    data = data.append("\n</table>");

    return data;
}

/*
 * Permet de calculer la largeur maximale qu'aura un bouton pour cette question.
 *
 * Utilis� pour avoir des boutons de m�mes dimensions.
 *
 * @param lg la langue des r�ponses
 * @return la largeur maximale d'un bouton
 */
int ResponseDeclared::getSizeResponseItem(Language lg)
{
    int maxWidth = 0;
    for(int i=0; i < m_responseItems.size(); i++)
    {
        //@TODO modifier le calcul car l� ce n'est pas pr�cis : pas d�pendant de la police ou de la taille
        int currentWidth = m_responseItems.at(i)->getLabel(lg).size() * 24;
        if(currentWidth > maxWidth)
            maxWidth = currentWidth;
    }

    return maxWidth;
}

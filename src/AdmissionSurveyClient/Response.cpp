#include "Response.h"

/*
 * Constructeur.
 *
 * @param type le type de la r�ponse
 */
Response::Response(ResponseType type)
{
    m_type = type;
}

/*
 * Destructeur.
 */
Response::~Response()
{
    qDeleteAll(m_responseItems);
}

/*
 * Retourne le type de la r�ponse.
 *
 * @return le type de la r�ponse
 */
ResponseType Response::getType()
{
    return m_type;
}

/*
 * Retourne l'�l�ment de r�ponse ayant le tag sp�cifi�.
 *
 * @param tag le tag sp�cifi�e
 * @return l'�l�ment de r�ponse ou NULL si l'�l�ment de r�ponse li� � ce le tag n'existe pas
 */
ResponseItem * Response::getResponseItem(QString tag)
{
    for(int i=0; i < m_responseItems.size(); i++)
    {
        ResponseItem *ri = m_responseItems.at(i);
        if(ri->getTag() == tag)
            return ri;
    }
    
    return NULL;
}

/*
 * Retourne tous les �l�ments de r�ponse.
 *
 * @return tous les �l�ments de r�ponse
 */
QVector<ResponseItem *> Response::getAllResponseItem()
{
    return QVector<ResponseItem *>(m_responseItems);
}

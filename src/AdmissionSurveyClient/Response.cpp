#include "Response.h"

/*
 * Constructeur.
 *
 * @param type le type de la réponse
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
 * Retourne le type de la réponse.
 *
 * @return le type de la réponse
 */
ResponseType Response::getType()
{
    return m_type;
}

/*
 * Retourne l'élément de réponse ayant le tag spécifié.
 *
 * @param tag le tag spécifiée
 * @return l'élément de réponse ou NULL si l'élément de réponse lié à ce le tag n'existe pas
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
 * Retourne tous les éléments de réponse.
 *
 * @return tous les éléments de réponse
 */
QVector<ResponseItem *> Response::getAllResponseItem()
{
    return QVector<ResponseItem *>(m_responseItems);
}

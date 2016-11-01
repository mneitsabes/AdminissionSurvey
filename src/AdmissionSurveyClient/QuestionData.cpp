#include "QuestionData.h"

/*
 * Constructeur.
 *
 * @param data les donn�es HTML
 * @param baseUrl la base pour les ressources
 */
QuestionData::QuestionData(QByteArray data, QUrl baseUrl)
{
    m_data = data;
    m_baseUrl = baseUrl;
}

/*
 * Retourne les donn�es HTML.
 *
 * @return les donn�es HTML
 */
QByteArray QuestionData::getData()
{
    return m_data;
}

/*
 * Retourne la base pour les ressources.
 *
 * @return la base pour les ressources
 */
QUrl QuestionData::getBaseUrl()
{
    return m_baseUrl;
}

#include "QuestionData.h"

/*
 * Constructeur.
 *
 * @param data les données HTML
 * @param baseUrl la base pour les ressources
 */
QuestionData::QuestionData(QByteArray data, QUrl baseUrl)
{
    m_data = data;
    m_baseUrl = baseUrl;
}

/*
 * Retourne les données HTML.
 *
 * @return les données HTML
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

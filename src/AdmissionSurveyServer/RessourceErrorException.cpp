#include "RessourceErrorException.h"

/*
 * Constructeur.
 *
 * @param type le type de la ressource
 * @param questionId l'ID de la question
 */
RessourceErrorException::RessourceErrorException(RessourceErrorType type, int questionId) throw()
{
    m_type = type;
    m_questionId = questionId;
}

/*
 * Destructeur.
 */
RessourceErrorException::~RessourceErrorException() throw() { }

/*
 * Retourne l'ID de la question
 *
 * @return l'ID
 */
int RessourceErrorException::getQuestionId()
{
    return m_questionId;
}

/*
 * Retourne le type de la ressource
 *
 * @return le type de la ressource
 */
RessourceErrorType RessourceErrorException::getType()
{
    return m_type;
}

/*
 * Retourne le titre de l'exception
 *
 * @return le titre de l'exception
 */
QString RessourceErrorException::getTitle()
{
    if(m_type == HTML_FILE)
        return tr("Erreur de chargement du fichier HTML");
    else
        return tr("Erreur de chargement");
}

/*
 * Retourne le message de l'exception
 *
 * @return le message de l'exception
 */
QString RessourceErrorException::getMessage()
{
     if(m_type == HTML_FILE)
        return tr("Impossible de lire le fichier HTML pour la question %1.").arg(m_questionId);
     else
        return tr("Impossible de lire le fichier");
}

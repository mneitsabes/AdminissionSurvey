#include "RessourceFormatException.h"

/*
 * Constructeur.
 *
 * @param tag le tag qui a posé probleme
 * @param type le type d'erreurs de format
*/
RessourceFormatException::RessourceFormatException(QString tag, RessourceFormatExceptionType type) throw()
{
    m_tag = tag;
    m_type = type;
    m_questionId = -1;
}

/*
 * Destructeur.
 */
RessourceFormatException::~RessourceFormatException() throw() { }

/*
 * Permet de définir l'ID de la question en erreur.
 *
 * @param questionId l'ID de la question
 */
void RessourceFormatException::setQuestionId(int questionId)
{
    m_questionId = questionId;
}

/*
 * Permet de définir la tag en cas d'une erreur "balise inconnue"
 *
 * @param tag la balise inconnue
 */
void RessourceFormatException::setTag(QString tag)
{
    m_tag = tag;
}

/*
 * Retourne le type de l'erreur de format.
 *
 * @return le type d'erreur de format
 */
RessourceFormatExceptionType RessourceFormatException::getType()
{
    return m_type;
}

/*
 * Retourne le numéro de la page.
 *
 * @return le numero de la page
 */
int RessourceFormatException::getQuestionId()
{
    return m_questionId;
}

/*
 * Retourne la balise inconnue.
 *
 * @return la balise inconnue
 */
QString RessourceFormatException::getTag()
{
    return m_tag;
}

/*
 * Retourne le titre de l'exception.
 *
 * @return le titre de l'exception
 */
QString RessourceFormatException::getTitle()
{
    return tr("Format du fichier XML incorrect");
}

/*
 * Retourne le message de l'exception.
 *
 * @return le message de l'exception
 */
QString RessourceFormatException::getMessage()
{
    QString s = "";

    if(m_questionId == -1)
        s += tr("Le format du fichier XML est erroné. ");
    else
        s += tr("Le format du fichier XML est erroné pour la question %1. ").arg(m_questionId);

    s += tr("La raison est la suivante :<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");

    switch(m_type)
    {
        case MULTI_DEF:
            s += tr("La balise <i>%1</i> a été trouvée plusieurs fois.").arg(m_tag);
        break;
        case FORMAT_VALUE:
            s += tr("Le format de la valeur pour la balise <i>%1</i> est incorrect.").arg(m_tag);
        break;
        case BAD_VALUE:
            s += tr("La valeur indiquée pour la balise <i>%1</i> est incorrecte.").arg(m_tag);
        break;
        case MISSING_TAG:
            s += tr("La balise <i>%1</i> est manquante.").arg(m_tag);
        break;
        case UNKNOWN_TAG:
            s += tr("Une balise inconnue a été trouvée : <i>%1</i>.").arg(m_tag);
        break;
        case ID_MISSING:
            s += tr("L'ID <i>%1</i> est manquant dans la liste des questions.").arg(m_tag);
        break;
        case MISSING_ATTRIBUTE:
            s += tr("Un attribut inconnu a été trouvé : <i>%1</i>.").arg(m_tag);
        break;
        case UNKNOWN_ATTRIBUTE:
            s += tr("L'attribut <i>%1</i> est manquant.").arg(m_tag);
        break;
    }

    return s;
}

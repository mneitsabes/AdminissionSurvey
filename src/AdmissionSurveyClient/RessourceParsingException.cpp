#include "RessourceParsingException.h"

/*
 * Constructeur.
 *
 * @param line le numéro de la ligne qui a provoqué l'erreur d'analyse
 */
RessourceParsingException::RessourceParsingException(int line) throw()
{
    m_line = line;
}

/*
 * Destructeur.
 */
RessourceParsingException::~RessourceParsingException() throw() { }

/*
 * Retourne le numéro de la ligne qui a provoqué l'erreur
 *
 * @return le numéro de la ligne
 */
int RessourceParsingException::getLine()
{
    return m_line;
}

/*
 * Retourne le titre de l'exception
 *
 * @return le titre de l'exception
 */
QString RessourceParsingException::getTitle()
{
    return tr("Erreur lors de l'analyse du fichier XML");
}

/*
 * Retourne le message de l'exception
 *
 * @return le message de l'exception
 */
QString RessourceParsingException::getMessage()
{
    if(m_line == -1)
        return tr("Ce fichier n'est pas un fichier de configuration.");
    else
        return tr("Le format du fichier XML est erroné à la ligne %1.").arg(m_line);
}

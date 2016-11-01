#include "RessourceOpenException.h"

/*
 * Constructeur.
 *
 * @param file le fichier
 */
RessourceOpenException::RessourceOpenException(QString file) throw() : RessourceException()
{
    m_file = file;
}

/*
 * Destructeur.
 */
RessourceOpenException::~RessourceOpenException() throw() { }

/*
 * Retourne le fichier.
 *
 * @return le fichier
 */
QString RessourceOpenException::getFile()
{
    return m_file;
}

/*
 * Retourne le titre de l'exception
 *
 * @return le titre de l'exception
 */
QString RessourceOpenException::getTitle()
{
    return tr("Erreur d'ouverture");
}

/*
 * Retourne le message de l'exception
 *
 * @return le message de l'exception
 */
QString RessourceOpenException::getMessage()
{
    if(m_file.trimmed().size() == 0)
        return tr("Pas de fichier XML fourni au programme.");
    else
        return tr("Impossible d'ouvrir le fichier XML <i>%1</i>.").arg(m_file);
}

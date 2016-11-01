#include "Question.h"

/*
 * Cr�� une question � partir de son ID.
 *
 * @param id
 */
Question::Question(int id)
{
    m_id = id;
    m_files = new QMap<Language, QString>();
}

/*
 * Destructeur.
 */
Question::~Question()
{
    m_files->clear();
    delete m_files;
    m_files = NULL;

    delete m_response;
    m_response = NULL;
}

/*
 * Ajoute un fichier pour la langue sp�cifi�e.
 *
 * Il n'est pas possible d'ajouter un fichier pour la lange "NONE".
 *
 * @lg la langue
 * @path le chemin du fichier
 */
void Question::addFile(Language lg, QString path)
{
    if(lg != NONE)
    {
        if(!m_files->contains(lg))
            m_files->insert(lg, path);
        else
            throw new IllegalArgumentException("Language already exists");
    }
    else
        throw new IllegalArgumentException("Language cannot be NONE");
}

/*
 * Permet de d�finir les r�ponses pour la question.
 *
 * @response les r�ponses
 */
void Question::setResponse(Response *response)
{
    m_response = response;
}

/*
 * Retourne l'ID de la question.
 *
 * @return l'ID
 */
int Question::getId()
{
    return m_id;
}

/*
 * Retourne les r�ponses � la question.
 *
 * @return les r�ponses
 */
Response * Question::getResponse()
{
    return m_response;
}

/*
 * Retourne la localisation du fichier contenant les donn�es HTML pour la langue sp�cifi�e.
 *
 * @lg la langue
 * @return la localisation du fichier
 */
QString Question::getPath(Language lg)
{
    if(lg != NONE)
    {
        if(m_files->contains(lg))
            return m_files->find(lg).value();
        else
            throw new IllegalArgumentException("No entry for this language");
    }
    else
        throw new IllegalArgumentException("Language cannot be NONE");
}

/*
 * Retourne le contenu HTML pour la langue sp�cifi�e.
 *
 * @lg la langue
 * @return le contenu HTML
 */
QByteArray Question::getHTML(Language lg)
{
    return Tools::readAllFile(getPath(lg));
}

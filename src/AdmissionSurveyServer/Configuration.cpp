#include "Configuration.h"

/*
 * Constructeur.
 *
 * Construis la configuration � partir du fichier XML pass�e en param�tre.
 *
 * @param configPath le fichier XML d�taillant la configuration
 */
Configuration::Configuration(QString configPath)
{
    m_outputItems = NULL;

    ConfigurationXML *configXML = new ConfigurationXML(configPath);
    m_outputItems = configXML->getOutputItems();
}

/*
 * Destructeur.
 */
Configuration::~Configuration()
{
    qDeleteAll(*m_outputItems);
    delete m_outputItems;
    m_outputItems = NULL;
}

/*
 * Retourne les �l�ments de sortie.
 *
 * @return les �l�ments de sortie
 */
QVector<OutputItem *> * Configuration::getOutputItems()
{
    return m_outputItems;
}

/*
 * Retourne le nombre de questions g�r�es au total par la configuration.
 */
int Configuration::getNbQuestions()
{
    int nbQuestions = 0;
    for(int i=0; i < m_outputItems->size(); i++)
        nbQuestions += m_outputItems->at(i)->getNbQuestionsAttached();

    return nbQuestions;
}

#include "Configuration.h"

/*
 * Constructeur.
 *
 * Construis la configuration à partir du fichier XML passée en paramètre.
 *
 * @param configPath le fichier XML détaillant la configuration
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
 * Retourne les éléments de sortie.
 *
 * @return les éléments de sortie
 */
QVector<OutputItem *> * Configuration::getOutputItems()
{
    return m_outputItems;
}

/*
 * Retourne le nombre de questions gérées au total par la configuration.
 */
int Configuration::getNbQuestions()
{
    int nbQuestions = 0;
    for(int i=0; i < m_outputItems->size(); i++)
        nbQuestions += m_outputItems->at(i)->getNbQuestionsAttached();

    return nbQuestions;
}

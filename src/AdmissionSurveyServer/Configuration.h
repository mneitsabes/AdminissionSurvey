#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QVector>
#include <QString>

#include "OutputItem.h"
#include "ConfigurationXML.h"

/*
 * Configuration du serveur.
 *
 * Permet de définir dans quel fichiers seront enregistrées les différentes réponses.
 */
class Configuration
{
    private:
        /* Les éléments de sortie */
        QVector<OutputItem *> *m_outputItems;

    public:
        Configuration(QString configPath);
        ~Configuration();
        QVector<OutputItem *> * getOutputItems();
        int getNbQuestions();
};

#endif // CONFIGURATION_H

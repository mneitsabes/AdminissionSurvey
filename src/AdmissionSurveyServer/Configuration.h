#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QVector>
#include <QString>

#include "OutputItem.h"
#include "ConfigurationXML.h"

/*
 * Configuration du serveur.
 *
 * Permet de d�finir dans quel fichiers seront enregistr�es les diff�rentes r�ponses.
 */
class Configuration
{
    private:
        /* Les �l�ments de sortie */
        QVector<OutputItem *> *m_outputItems;

    public:
        Configuration(QString configPath);
        ~Configuration();
        QVector<OutputItem *> * getOutputItems();
        int getNbQuestions();
};

#endif // CONFIGURATION_H

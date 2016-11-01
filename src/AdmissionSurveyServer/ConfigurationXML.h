#ifndef CONFIGURATIONXML_H
#define CONFIGURATIONXML_H

#include <QFile>
#include <QDomNode>
#include <QDomElement>
#include <QDomDocument>
#include <QVector>
#include <QDebug>

#include "RessourceException.h"
#include "RessourceOpenException.h"
#include "RessourceParsingException.h"
#include "RessourceFormatException.h"
#include "RessourceErrorException.h"
#include "Configuration.h"
#include "OutputItem.h"

/*
 * Permet de lire le fichier XML contenant la configuration du serveur et de construire une configuration valide.
 */
class ConfigurationXML
{
    private:
        /* Les éléments de sortie */
        QVector<OutputItem *> *m_outputItems;

        /* Code d'erreur */
        int m_error;

    public:
        ConfigurationXML(QString path);
        ~ConfigurationXML();
        void parse(QDomDocument *doc);
        QVector<OutputItem *> *getOutputItems();

    private:
        OutputItem * getOutputItem(QString filePath);
};

#endif // CONFIGURATIONXML_H

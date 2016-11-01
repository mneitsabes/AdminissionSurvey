#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QVector>
#include <QRegExp>
#include <QColor>

#include "Question.h"
#include "RessourceException.h"
#include "RessourceFormatException.h"
#include "IllegalArgumentException.h"
#include "EngineXML.h"

/*
 * Configuration du DBManager.
 */
struct ConfigurationDBManager
{
    /* Chemin du fichier lors d'une sauvegarde r�ussie */
    QString pathNormal;

    /* Chemin du fichier lors d'une sauvegarde ayant provoqu� une erreur */
    QString pathError;
};

/*
 * Configuration du r�seau.
 */
struct ConfigurationNetwork
{
    /* Adresse IP du serveur */
    QString serverAddr;

    /* Port sur lequel �coute le serveur */
    int serverPort;
};

/*
 * Configuration du GUI.
 */
struct ConfigurationGUI
{
    /* Police utilis�e dans les fichiers HTML */
    QString fontFamily;

    /* Taille des bordures */
    int borderSize;

    /* Couleur des bordures */
    QColor borderColor;
};

/*
 * Configuration d'AdmissionSurvey, aussi bien sur le moteur que sur la vue.
 *
 * R�cup�re et cr�� la configuration � partir du fichier XML la d�crivant (sur base de EngineXML).
 */
class Configuration
{
    private:
        /* Les diff�rentes questions */
        QVector<Question *> *m_questions;

        /* La configuration r�seau */
        ConfigurationNetwork m_network;

        /* La configuration GUI */
        ConfigurationGUI m_gui;

        /* La configuration du DBManager */
        ConfigurationDBManager m_dbManager;

        /* Mode de d�veloppement : si vrai -> mode fen�tr� et curseur */
        bool m_devMode;

    public:
        Configuration(QString pathXML);
        ~Configuration();
        void setQuestions(QVector<Question *> *questions);
        void setNetwork(QString addr, int port);
        void setGUI(QString fontFamily, int borderSize, QString borderColor);
        void setDBManager(QString pathNormal, QString pathError);
        void setDevMode(bool devMode);
        int getQuestionsSize();
        ConfigurationNetwork getNetwork();
        ConfigurationGUI getGUI();
        ConfigurationDBManager getDBManager();
        bool isDevMode();
        Question * getQuestion(int id);

    private:
        bool isIdExists(int id);
        void addQuestion(Question *question);
};

#endif // CONFIGURATION_H

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
    /* Chemin du fichier lors d'une sauvegarde réussie */
    QString pathNormal;

    /* Chemin du fichier lors d'une sauvegarde ayant provoqué une erreur */
    QString pathError;
};

/*
 * Configuration du réseau.
 */
struct ConfigurationNetwork
{
    /* Adresse IP du serveur */
    QString serverAddr;

    /* Port sur lequel écoute le serveur */
    int serverPort;
};

/*
 * Configuration du GUI.
 */
struct ConfigurationGUI
{
    /* Police utilisée dans les fichiers HTML */
    QString fontFamily;

    /* Taille des bordures */
    int borderSize;

    /* Couleur des bordures */
    QColor borderColor;
};

/*
 * Configuration d'AdmissionSurvey, aussi bien sur le moteur que sur la vue.
 *
 * Récupère et créé la configuration à partir du fichier XML la décrivant (sur base de EngineXML).
 */
class Configuration
{
    private:
        /* Les différentes questions */
        QVector<Question *> *m_questions;

        /* La configuration réseau */
        ConfigurationNetwork m_network;

        /* La configuration GUI */
        ConfigurationGUI m_gui;

        /* La configuration du DBManager */
        ConfigurationDBManager m_dbManager;

        /* Mode de développement : si vrai -> mode fenêtré et curseur */
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

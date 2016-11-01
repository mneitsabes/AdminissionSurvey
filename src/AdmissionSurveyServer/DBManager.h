#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <iostream>

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFile>

#include "ResponsesSet.h"
#include "IllegalArgumentException.h"
#include "Configuration.h"
#include "OutputItem.h"

extern ostream cout;

/* Valeur pour une question sans r�ponse */
#define DBMANAGER_NOREPONSE_SEQUENCE "/"

/*
 * Le gestionnaire de sauvegarde des r�sultats dans des fichiers locaux.
 *
 * Permet de sauver les r�sultats d'une session dans des fichiers locaux au format CSV. Voici la structure d'une ligne :
 *    <ID unique de session>;<date format dd/mm/yyy>;<heure format hh:mm>;<valeur Q1>;<valeur Q2>;<valeur Qn>;<num�ro d'admission>
 *
 */
class DBManager
{
    private:
        /* Le chemin du fichier des r�sultats*/
        QString m_path;

        /* La configuration */
        Configuration *m_configuration;

    public:
        DBManager(Configuration *configuration, QString path);
        void addResponsesSet(ResponsesSet responsesSet);

    private:
        void appendToFile(QString pathFile, QString uniqueId, QDateTime dateTime, QVector<QuestionResponse> responses, QString numAdm);
        QuestionResponse getQuestionResponseFromId(QVector<QuestionResponse> qrs, int questionId);
};

#endif // DBMANAGER_H

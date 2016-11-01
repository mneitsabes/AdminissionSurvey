#ifndef ADMISSIONSURVEYSERVER_H
#define ADMISSIONSURVEYSERVER_H

#include <QTcpServer>
#include <QVector>

#include "QuestionResponse.h"
#include "ClientSocket.h"
#include "ResponsesSet.h"
#include "DBManager.h"
#include "IllegalArgumentException.h"
#include "Configuration.h"

/* Port sur lequel le serveur accepte les connexions */
#define ADMISSIONSURVEYSERVER_PORT 5728

/*
 * Le serveur centralisant les résultats des différents clients AdmissionSurvey.
 *
 * Le serveur accepte les connexions TCP sur le port défini par ADMISSIONSURVEYSERVER_PORT.
 * Dès qu'une connexion est établie, un objet de type ClientSocket est instancié pour gérer la connexion.
 */
class AdmissionSurveyServer : public QTcpServer
{
    Q_OBJECT

    private:
        /* Le gestionnaire de sauvegardes */
        DBManager *m_dbManager;

        /* La configuration */
        Configuration *m_configuration;

    public:
        AdmissionSurveyServer(Configuration *configuration, QString path, QObject *parent = 0);
        ~AdmissionSurveyServer();
        void addResponsesSet(ResponsesSet responsesSet);

    private:
        void incomingConnection(int socketId);
};

#endif // ADMISSIONSURVEYSERVER_H

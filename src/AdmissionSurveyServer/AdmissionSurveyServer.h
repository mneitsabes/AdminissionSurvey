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
 * Le serveur centralisant les r�sultats des diff�rents clients AdmissionSurvey.
 *
 * Le serveur accepte les connexions TCP sur le port d�fini par ADMISSIONSURVEYSERVER_PORT.
 * D�s qu'une connexion est �tablie, un objet de type ClientSocket est instanci� pour g�rer la connexion.
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

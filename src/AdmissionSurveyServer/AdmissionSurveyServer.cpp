#include "AdmissionSurveyServer.h"

/*
 * Constructeur.
 *
 * @param path le chemin du fichier dans lequel sera enregistr� les r�sultats
 * @parent l'objet parent
 */
AdmissionSurveyServer::AdmissionSurveyServer(Configuration *configuration, QString path, QObject *parent) : QTcpServer(parent)
{
    m_configuration = configuration;

    if(path.isEmpty())
        throw new IllegalArgumentException("Path cannot be empty");

    m_dbManager = new DBManager(configuration, path);
}

/*
 * Destructeur.
 */
AdmissionSurveyServer::~AdmissionSurveyServer()
{
    delete m_dbManager;
    m_dbManager = NULL;

    delete m_configuration;
    m_configuration = NULL;
}

/*
 * Est invoqu�e lorsqu'une nouvelle connexion se produit.
 *
 * Un objet de type ClientSocket est cr�� pour traiter la connexion.
 *
 * @param socketId l'ID du descripteur du socket
 */
void AdmissionSurveyServer::incomingConnection(int socketId)
{
    ClientSocket *socket = new ClientSocket(this);
    socket->setSocketDescriptor(socketId);
}

/*
 * Permet aux clients de demander l'�criture d'un ensemble de r�ponses dans la sauvegarde.
 *
 * @param responsesSet l'ensemble des r�ponses envoy�es par le client
 */
void AdmissionSurveyServer::addResponsesSet(ResponsesSet responsesSet)
{
    m_dbManager->addResponsesSet(responsesSet);
}

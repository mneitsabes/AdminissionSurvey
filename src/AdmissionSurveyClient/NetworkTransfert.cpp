#include "NetworkTransfert.h"

/*
 * Constructeur.
 *
 * @param config la configuration
 */
NetworkTransfert::NetworkTransfert(Configuration *config)
{
    m_config = config;
    m_listeners = new QVector<NetworkTransfertListener *>();
    m_recorderSession = NULL;

    //on se lie au socket
    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(sendRecord()));
    connect(&m_tcpSocket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readResponse()));
    connect(&m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
}

/*
 * Destructeur.
 */
NetworkTransfert::~NetworkTransfert()
{
    m_listeners->clear(); //ne d�truit pas les �couteurs ici
    delete m_listeners;
    m_listeners = NULL;

    m_recorderSession = NULL; //ne d�truit pas l'enregistreur de session ici
    m_config = NULL;
}

/*
 * Ajout d'un �couteur.
 *
 * @param listener l'�couteur
 */
void NetworkTransfert::addNetworkTransfertListener(NetworkTransfertListener *listener)
{
    if(!m_listeners->contains(listener)) //ajouter que si l'�couteur n'�coute pas d�j�
        m_listeners->append(listener);
}

/*
 * Supprime un �couteur.
 *
 * @param listener l'�couteur
 */
void NetworkTransfert::removeNetworkTransfertListener(NetworkTransfertListener *listener)
{
    int pos = m_listeners->indexOf(listener);
    if(pos != -1)
        m_listeners->remove(pos);
}

/*
 * Permet d'envoyer les r�sultats au serveur.
 *
 * Les r�sultats sont pris � partir de l'enregistreur de session
 *
 * @param recorderSession l'enregistreur de session
 */
void NetworkTransfert::sendRecords(RecorderSession *recorderSession)
{
    if(recorderSession == NULL)
        throw new IllegalArgumentException("RecorderSession cannot be NULL");

    if(m_tcpSocket.isOpen())
        throw new IllegalArgumentException("Socket is used");

    //on sauve l'enregistreur de session et l'on lance la question au serveur
    m_recorderSession = recorderSession;
    connectToServer();
}

/*
 * Lance la connexion au serveur.
 *
 * La connexion est asynchrone. La m�thode sendRecord() sera invoqu�e lorsque la connexion sera �tablie.
 */
void NetworkTransfert::connectToServer()
{
    //re-initialisation de la taille des blocs � 0
    m_nextBlockSize = 0;

    //on re-initialise l'indicateur d'erreur
    m_errorReported = false;

    //lancement de la connexion (asynchrone)
    m_tcpSocket.connectToHost(m_config->getNetwork().serverAddr, m_config->getNetwork().serverPort);
}

/*
 * Rapporte d'une erreur r�seau.
 *
 * Le socket est ferm� pour etre r�utilis� et les �couteurs sont pr�venus.
 *
 * @param errorMsg le message d'erreur
 */
void NetworkTransfert::reportError(QString errorMsg)
{
    if(!m_errorReported) //seulement si une erreur n'a pas encore �t� report�e
    {
        m_errorReported = true;

        //le socket est ferm� pour etre r�utilis� par la session suivante
        m_tcpSocket.close();

        for(int i=0; i < m_listeners->size(); i++)
            m_listeners->at(i)->recordTransfertError(m_recorderSession->getUniqueId(), errorMsg);
    }
}

/*
 * Permet d'envoyer les donn�es de l'enregistreur de session.
 *
 * Est appel�e automatiquement lorsque la connexion au serveur est �tablie. La structure est d�finie dans la description de cette classe.
 */
void NetworkTransfert::sendRecord()
{
    //cr�ation du paquet
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0);                     //taille du block
    out << quint8(0);                      //0 : envoi d'un record de session
    out << quint32(m_recorderSession->getUniqueId().length());   //ID unique sur 32 caract�res
    out << m_recorderSession->getUniqueId();

    QVector<Record *> *records = m_recorderSession->getRecordsCompletedAndOrdered();
    for(int i=0; i < records->size(); i++) //pour chaque question
    {
        out << quint8('Q'); //Q : question

        if(records->at(i) != NULL) //record trouv� pour la question
        {
            out << quint32(records->at(i)->getQuestion()->getId());
            out << quint32(records->at(i)->getValue().length());
            out << records->at(i)->getValue();
        }
        else //pas de record pour la question courante (ID : i+1)
        {
            out << quint32(i+1); //ID courant = i+1
            out << quint32(0);
        }
    }

    delete records;

    out << quint8('A');                                         //A : num�ro d'admission
    out << quint32(m_recorderSession->getNumAdm().length());    //longueur du num�ro d'admission
    out << m_recorderSession->getNumAdm();                      //valeur du num�ro d'admission
    out << quint16(0xFFFF);                                     //fin de block

    out.device()->seek(0);                                      //on se replace au d�but
    out << quint16(block.size() - sizeof(quint16));             //on place la taille du block au d�but de celui-ci

    m_tcpSocket.write(block);
}

/*
 * Permet de traiter les donn�es envoy�es du serveur.
 *
 * Est appel�e automatiquement lorsque le serveur renvoit des donn�es. La structure est d�finie dans la description de cette classe.
 */
void NetworkTransfert::readResponse()
{
    QDataStream in(&m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_5);

    forever {
        if(m_nextBlockSize == 0xFFFF) //il n'y a plus de donn�es � lire
            return ;

        if(m_nextBlockSize == 0) //taille du block inconnue
        {
            if(m_tcpSocket.bytesAvailable() < sizeof(quint16)) //les 16 premiers bits contenant la taille du block sont d�j� arriv�s ?
                break;

            in >> m_nextBlockSize; //la taille du block � r�cup�rer
        }

        if(m_tcpSocket.bytesAvailable() < m_nextBlockSize) //les donn�es du block sont d�j� arriv�es ?
            break;

        //lecture
        quint8 responseCode;
        quint32 uniqueIdLength;
        QString uniqueId;
        quint16 endFlag;

        in >> responseCode;
        in >> uniqueIdLength;
        in >> uniqueId;
        in >> endFlag;

        if(endFlag == 0xFFFF && responseCode == 0 && uniqueId == m_recorderSession->getUniqueId()) //le serveur a bien re�u les donn�es, on peut cl�turer
        {
            m_nextBlockSize = 0xFFFF;
            m_tcpSocket.close();
        }
        else //une erreur r�seau s'est produite
        {
            for(int i=0; i < m_listeners->size(); i++)
                m_listeners->at(i)->recordTransfertError(m_recorderSession->getUniqueId(), "Packet corrumpted");
        }

        m_nextBlockSize = 0;
    }
}

/*
 * Traite la fin de connexion.
 *
 * Lorsque le client ferme le socket, il envoit une demande au serveur qui clot le socket.
 * Cette m�thode est donc invoqu�e m�me dans le cas o� le client r�alise la demande de fermeture.
 * Si m_nextBlockSize a la valeur 0xFFFF, il s'agit d'une demande du client et il ne s'agit donc pas d'une erreur.
 * Dans ce cas, les �couteurs sont pr�venus de la bonne ex�cution d'envoi au serveur. Dans le cas contraire, l'erreur est rapport�e (voir reportError()).
 */
void NetworkTransfert::connectionClosedByServer()
{
    if(m_nextBlockSize != 0xFFFF)
        reportError("Connection closed by server");
    else
    {
        for(int i=0; i < m_listeners->size(); i++)
            m_listeners->at(i)->recordTransfertSuccessful(m_recorderSession->getUniqueId());
    }
}

/*
 * Est invoqu�e lors d'une erreur r�seau.
 *
 * L'erreur est rapport�e (voir reportError()).
 */
void NetworkTransfert::error()
{
    reportError(m_tcpSocket.errorString());
}

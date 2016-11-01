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
    m_listeners->clear(); //ne détruit pas les écouteurs ici
    delete m_listeners;
    m_listeners = NULL;

    m_recorderSession = NULL; //ne détruit pas l'enregistreur de session ici
    m_config = NULL;
}

/*
 * Ajout d'un écouteur.
 *
 * @param listener l'écouteur
 */
void NetworkTransfert::addNetworkTransfertListener(NetworkTransfertListener *listener)
{
    if(!m_listeners->contains(listener)) //ajouter que si l'écouteur n'écoute pas déjà
        m_listeners->append(listener);
}

/*
 * Supprime un écouteur.
 *
 * @param listener l'écouteur
 */
void NetworkTransfert::removeNetworkTransfertListener(NetworkTransfertListener *listener)
{
    int pos = m_listeners->indexOf(listener);
    if(pos != -1)
        m_listeners->remove(pos);
}

/*
 * Permet d'envoyer les résultats au serveur.
 *
 * Les résultats sont pris à partir de l'enregistreur de session
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
 * La connexion est asynchrone. La méthode sendRecord() sera invoquée lorsque la connexion sera établie.
 */
void NetworkTransfert::connectToServer()
{
    //re-initialisation de la taille des blocs à 0
    m_nextBlockSize = 0;

    //on re-initialise l'indicateur d'erreur
    m_errorReported = false;

    //lancement de la connexion (asynchrone)
    m_tcpSocket.connectToHost(m_config->getNetwork().serverAddr, m_config->getNetwork().serverPort);
}

/*
 * Rapporte d'une erreur réseau.
 *
 * Le socket est fermé pour etre réutilisé et les écouteurs sont prévenus.
 *
 * @param errorMsg le message d'erreur
 */
void NetworkTransfert::reportError(QString errorMsg)
{
    if(!m_errorReported) //seulement si une erreur n'a pas encore été reportée
    {
        m_errorReported = true;

        //le socket est fermé pour etre réutilisé par la session suivante
        m_tcpSocket.close();

        for(int i=0; i < m_listeners->size(); i++)
            m_listeners->at(i)->recordTransfertError(m_recorderSession->getUniqueId(), errorMsg);
    }
}

/*
 * Permet d'envoyer les données de l'enregistreur de session.
 *
 * Est appelée automatiquement lorsque la connexion au serveur est établie. La structure est définie dans la description de cette classe.
 */
void NetworkTransfert::sendRecord()
{
    //création du paquet
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_5);

    out << quint16(0);                     //taille du block
    out << quint8(0);                      //0 : envoi d'un record de session
    out << quint32(m_recorderSession->getUniqueId().length());   //ID unique sur 32 caractères
    out << m_recorderSession->getUniqueId();

    QVector<Record *> *records = m_recorderSession->getRecordsCompletedAndOrdered();
    for(int i=0; i < records->size(); i++) //pour chaque question
    {
        out << quint8('Q'); //Q : question

        if(records->at(i) != NULL) //record trouvé pour la question
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

    out << quint8('A');                                         //A : numéro d'admission
    out << quint32(m_recorderSession->getNumAdm().length());    //longueur du numéro d'admission
    out << m_recorderSession->getNumAdm();                      //valeur du numéro d'admission
    out << quint16(0xFFFF);                                     //fin de block

    out.device()->seek(0);                                      //on se replace au début
    out << quint16(block.size() - sizeof(quint16));             //on place la taille du block au début de celui-ci

    m_tcpSocket.write(block);
}

/*
 * Permet de traiter les données envoyées du serveur.
 *
 * Est appelée automatiquement lorsque le serveur renvoit des données. La structure est définie dans la description de cette classe.
 */
void NetworkTransfert::readResponse()
{
    QDataStream in(&m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_5);

    forever {
        if(m_nextBlockSize == 0xFFFF) //il n'y a plus de données à lire
            return ;

        if(m_nextBlockSize == 0) //taille du block inconnue
        {
            if(m_tcpSocket.bytesAvailable() < sizeof(quint16)) //les 16 premiers bits contenant la taille du block sont déjà arrivés ?
                break;

            in >> m_nextBlockSize; //la taille du block à récupérer
        }

        if(m_tcpSocket.bytesAvailable() < m_nextBlockSize) //les données du block sont déjà arrivées ?
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

        if(endFlag == 0xFFFF && responseCode == 0 && uniqueId == m_recorderSession->getUniqueId()) //le serveur a bien reçu les données, on peut clôturer
        {
            m_nextBlockSize = 0xFFFF;
            m_tcpSocket.close();
        }
        else //une erreur réseau s'est produite
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
 * Cette méthode est donc invoquée même dans le cas où le client réalise la demande de fermeture.
 * Si m_nextBlockSize a la valeur 0xFFFF, il s'agit d'une demande du client et il ne s'agit donc pas d'une erreur.
 * Dans ce cas, les écouteurs sont prévenus de la bonne exécution d'envoi au serveur. Dans le cas contraire, l'erreur est rapportée (voir reportError()).
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
 * Est invoquée lors d'une erreur réseau.
 *
 * L'erreur est rapportée (voir reportError()).
 */
void NetworkTransfert::error()
{
    reportError(m_tcpSocket.errorString());
}

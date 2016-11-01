#include "ClientSocket.h"

/*
 * Constructeur.
 *
 * Il faut que le parent soit l'instance du serveur (AdmissionSurveyServer).
 *
 * @param parent l'objet parent
 */
ClientSocket::ClientSocket(QObject *parent) : QTcpSocket(parent)
{
    //si l'objet
    m_server = dynamic_cast<AdmissionSurveyServer *>(parent);
    if(m_server == NULL)
        throw new IllegalArgumentException("Parent must be AdmissionSurveyServer");

    m_nextBlockSize = 0;

    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater())); //libère automatique l'objet
}

/*
 * Destructeur.
 */
ClientSocket::~ClientSocket()
{
    m_server = NULL; //ce n'est pas un enfant qui détruit le serveur
}

/*
 * Lis les données envoyées par le client.
 *
 * La structure du paquet est défini dans la documentation de cette classe.
 */
void ClientSocket::readClient()
{
    //initialisation
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_5);

    if(m_nextBlockSize == 0) //taille du block inconnue
    {
        if(bytesAvailable() < sizeof(quint16)) //les 16 premiers bits contenant la taille du block sont déjà arrivés ?
            return ;

        in >> m_nextBlockSize; //la taille du block à récupérer
    }

    if(bytesAvailable() < m_nextBlockSize) //les données du block sont déjà arrivées ?
        return ;

    //lecture de l'en-tete du paquet contenant le mode et l'ID unique de session
    ResponsesSet responsesSet;

    quint8 mode;
    quint32 uniqueIdLength;
    QString uniqueId;

    in >> mode;
    in >> uniqueIdLength;
    in >> uniqueId;

    responsesSet.setUniqueId(uniqueId);

    //on lit la suite
    quint8 catData;
    in >> catData;

    while(catData == 'Q') //s'agit-il d'une réponse à une question ?
    {
        quint32 questionId;
        quint32 recordValueLength;
        QString recordValue;

        in >> questionId;
        in >> recordValueLength;

        if(recordValueLength > 0) //l'utilisateur a répondu à la question, une valeur de réponse est donc présente
            in >> recordValue;
        else //pas de réponse pour cette question, on utilise la valeur de DBMANAGER_NOREPONSE_SEQUENCE à la place
            recordValue = DBMANAGER_NOREPONSE_SEQUENCE;

        responsesSet.addResponse(questionId, recordValue);

        //on lit la suite
        in >> catData;
    }

    //il n'y a plus de réponses à lire, on passe au numéro d'admission
    quint32 numAdmLength;
    QString numAdm;
    quint16 endFlag;

    in >> numAdmLength;
    in >> numAdm;
    in >> endFlag;

    responsesSet.setNumAdm(numAdm);

    if(endFlag == 0xFFFF) //on a bien tout reçu
        m_server->addResponsesSet(responsesSet);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << quint16(0);

    if(endFlag == 0xFFFF) //on a bien tout reçu
    {
        out << quint8(0);
        out << quint32(uniqueId.length());
        out << uniqueId;
        out << quint16(0xFFFF);
    }
    else //en cas d'erreur, on prévient le client
        out << quint8(1);

    out << quint16(0xFFFF);

    out.device()->seek(0);                            //on se replace au début
    out << quint16(block.size() - sizeof(quint16));   //on place la taille du block au début de celui-ci

    write(block);
}

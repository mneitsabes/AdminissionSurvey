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
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater())); //lib�re automatique l'objet
}

/*
 * Destructeur.
 */
ClientSocket::~ClientSocket()
{
    m_server = NULL; //ce n'est pas un enfant qui d�truit le serveur
}

/*
 * Lis les donn�es envoy�es par le client.
 *
 * La structure du paquet est d�fini dans la documentation de cette classe.
 */
void ClientSocket::readClient()
{
    //initialisation
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_5);

    if(m_nextBlockSize == 0) //taille du block inconnue
    {
        if(bytesAvailable() < sizeof(quint16)) //les 16 premiers bits contenant la taille du block sont d�j� arriv�s ?
            return ;

        in >> m_nextBlockSize; //la taille du block � r�cup�rer
    }

    if(bytesAvailable() < m_nextBlockSize) //les donn�es du block sont d�j� arriv�es ?
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

    while(catData == 'Q') //s'agit-il d'une r�ponse � une question ?
    {
        quint32 questionId;
        quint32 recordValueLength;
        QString recordValue;

        in >> questionId;
        in >> recordValueLength;

        if(recordValueLength > 0) //l'utilisateur a r�pondu � la question, une valeur de r�ponse est donc pr�sente
            in >> recordValue;
        else //pas de r�ponse pour cette question, on utilise la valeur de DBMANAGER_NOREPONSE_SEQUENCE � la place
            recordValue = DBMANAGER_NOREPONSE_SEQUENCE;

        responsesSet.addResponse(questionId, recordValue);

        //on lit la suite
        in >> catData;
    }

    //il n'y a plus de r�ponses � lire, on passe au num�ro d'admission
    quint32 numAdmLength;
    QString numAdm;
    quint16 endFlag;

    in >> numAdmLength;
    in >> numAdm;
    in >> endFlag;

    responsesSet.setNumAdm(numAdm);

    if(endFlag == 0xFFFF) //on a bien tout re�u
        m_server->addResponsesSet(responsesSet);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << quint16(0);

    if(endFlag == 0xFFFF) //on a bien tout re�u
    {
        out << quint8(0);
        out << quint32(uniqueId.length());
        out << uniqueId;
        out << quint16(0xFFFF);
    }
    else //en cas d'erreur, on pr�vient le client
        out << quint8(1);

    out << quint16(0xFFFF);

    out.device()->seek(0);                            //on se replace au d�but
    out << quint16(block.size() - sizeof(quint16));   //on place la taille du block au d�but de celui-ci

    write(block);
}

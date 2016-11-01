#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QString>

#include "AdmissionSurveyServer.h"
class AdmissionSurveyServer;
#include "ResponsesSet.h"
#include "IllegalArgumentException.h"
#include "DBManager.h"

/*
 * G�re une connexion d'un client.
 *
 * Les donn�es sont transf�r�es via TCP par le client. Une fois les donn�es sauvegard�es, une confirmation est renvoy�e au client.
 *
 * Voici la structure d'un paquet que le serveur re�oit d'un client pour l'envoi des r�sultats :
 *   - [sur 2 octets - quint16] la taille des donn�es qui suivent = la longueur du paquet apr�s les 16 premiers bits
 *   - [sur 1 octets - quint8]  le type de paquet = 0 pour l'envoi un record de session (1 seul type pour l'instant)
 *   - [sur 4 octets - quint32] la longueur de l'ID unique
 *   - [sur x octets - QString] l'ID unique sous forme de QString
 *   POUR CHAQUE QUESTION
 *      - [sur 1 octet - quint8] Q = pour question
 *      SI LA QUESTION A ETE POSEE
 *         - [sur 4 octets - quint32] l'ID de la question
 *         - [sur 4 octets - quint32] la longueur de la valeur de la r�ponse s�lectionn�e
 *         - [sur x octets - QString] la valeur de la r�ponse s�lectionn�e
 *      SINON SI LA QUESTION N'A PAS ETE POSEE (EX: SAUT DE QUESTION)
 *         - [sur 4 octets - quint32] l'ID de la question
 *         - [sur 4 octets - quint32] 0
 *   - [sur 1 octet - quint8]   A = pour le num�ro d'admission
 *   - [sur 4 octets - quint32] la longueur du num�ro d'admission
 *   - [sur x octets - QString] le num�ro d'admission
 *   - [sur 2 octets - quint16] 0xFFFF = fin de paquet
 *
 * Voici la structure d'un paquet de conformation :
 *   - [sur 2 octets - quint16] la taille des donn�es qui suivent = la longueur du paquet apr�s les 16 premiers bits
 *   - [sur 1 octets - quint8]  le type de paquet = 0 pour la bonne r�ception (1 seul type pour l'instant)
 *   - [sur 4 octets - quint32] la longueur de l'ID unique
 *   - [sur x octets - QString] l'ID unique sous forme de QString (le m�me que lors de l'envoi)
 *   - [sur 2 octets - quint16] 0xFFFF = fin de paquet
 */
class ClientSocket : public QTcpSocket
{
    Q_OBJECT

    private:
        /* La taille du block � lire (0: inconnu, >0: nb octets � lire, 0xFFFF: plus de donn�es � lire) */
        quint16 m_nextBlockSize;

        /* L'instance du serveur */
        AdmissionSurveyServer *m_server;

    public:
        ClientSocket(QObject *parent = 0);
        ~ClientSocket();

    private slots:
        void readClient();
};

#endif // CLIENTSOCKET_H

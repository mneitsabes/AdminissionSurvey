#ifndef NETWORKTRANSFERT_H
#define NETWORKTRANSFERT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QVector>
#include <QDebug>

#include "RecorderSession.h"
#include "Record.h"
#include "NetworkTransfertListener.h"
#include "Configuration.h"

/*
 * Module de transfert r�seau des r�ponses au serveur d'AdmissionSurvey.
 *
 * Les donn�es sont transf�r�es par TCP au serveur enregistrant les r�ponses. Le client envoit un paquet contenant les r�ponses et le num�ro d'admission.
 * Une fois que le serveur a proc�d� � l'enregistrement, il renvoit un paquet confirmant ou non la bonne r�ception des donn�es.
 * Les �couteurs de ce module (principalement Engine) seront avertis en cas d'envoi r�ussi ou �chou� (voir NetworkTransfertListener).
 *
 * Les r�ponses sont sorties de l'enregistreur de r�ponse. Il faut �galement envoy� une information pour les questions qui n'ont pas �t� pos�es (saut de question).
 *
 * Voici la structure d'un paquet d'�mission des r�sultats :
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
 * Voici la structure d'un paquet de r�ception (confirmation du serveur) :
 *   - [sur 2 octets - quint16] la taille des donn�es qui suivent = la longueur du paquet apr�s les 16 premiers bits
 *   - [sur 1 octets - quint8]  le type de paquet = 0 pour la bonne r�ception (1 seul type pour l'instant)
 *   - [sur 4 octets - quint32] la longueur de l'ID unique
 *   - [sur x octets - QString] l'ID unique sous forme de QString (le m�me que lors de l'envoi)
 *   - [sur 2 octets - quint16] 0xFFFF = fin de paquet
 */
class NetworkTransfert : public QObject
{
    Q_OBJECT

    private:
        /* La configuration */
        Configuration *m_config;

        /* Les �couteurs */
        QVector<NetworkTransfertListener *> *m_listeners;

        /* Le socket TCP */
        QTcpSocket m_tcpSocket;

        /* La taille du block � lire (0: inconnu, >0: nb octets � lire, 0xFFFF: plus de donn�es � lire) */
        quint16 m_nextBlockSize;

        /* L'enregistreur de session duquel lire les r�ponses */
        RecorderSession *m_recorderSession;

        /* Indique si une erreur a d�j� �t� report�e */
        bool m_errorReported;

    public:
        NetworkTransfert(Configuration *config);
        ~NetworkTransfert();
        void sendRecords(RecorderSession *recorderSession);
        void addNetworkTransfertListener(NetworkTransfertListener *listener);
        void removeNetworkTransfertListener(NetworkTransfertListener *listener);

    private:
        void connectToServer();
        void reportError(QString errorMsg);

    private slots:
        void sendRecord();
        void readResponse();
        void connectionClosedByServer();
        void error();
};

#endif // NETWORKTRANSFERT_H

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
 * Module de transfert réseau des réponses au serveur d'AdmissionSurvey.
 *
 * Les données sont transférées par TCP au serveur enregistrant les réponses. Le client envoit un paquet contenant les réponses et le numéro d'admission.
 * Une fois que le serveur a procédé à l'enregistrement, il renvoit un paquet confirmant ou non la bonne réception des données.
 * Les écouteurs de ce module (principalement Engine) seront avertis en cas d'envoi réussi ou échoué (voir NetworkTransfertListener).
 *
 * Les réponses sont sorties de l'enregistreur de réponse. Il faut également envoyé une information pour les questions qui n'ont pas été posées (saut de question).
 *
 * Voici la structure d'un paquet d'émission des résultats :
 *   - [sur 2 octets - quint16] la taille des données qui suivent = la longueur du paquet après les 16 premiers bits
 *   - [sur 1 octets - quint8]  le type de paquet = 0 pour l'envoi un record de session (1 seul type pour l'instant)
 *   - [sur 4 octets - quint32] la longueur de l'ID unique
 *   - [sur x octets - QString] l'ID unique sous forme de QString
 *   POUR CHAQUE QUESTION
 *      - [sur 1 octet - quint8] Q = pour question
 *      SI LA QUESTION A ETE POSEE
 *         - [sur 4 octets - quint32] l'ID de la question
 *         - [sur 4 octets - quint32] la longueur de la valeur de la réponse sélectionnée
 *         - [sur x octets - QString] la valeur de la réponse sélectionnée
 *      SINON SI LA QUESTION N'A PAS ETE POSEE (EX: SAUT DE QUESTION)
 *         - [sur 4 octets - quint32] l'ID de la question
 *         - [sur 4 octets - quint32] 0
 *   - [sur 1 octet - quint8]   A = pour le numéro d'admission
 *   - [sur 4 octets - quint32] la longueur du numéro d'admission
 *   - [sur x octets - QString] le numéro d'admission
 *   - [sur 2 octets - quint16] 0xFFFF = fin de paquet
 *
 * Voici la structure d'un paquet de réception (confirmation du serveur) :
 *   - [sur 2 octets - quint16] la taille des données qui suivent = la longueur du paquet après les 16 premiers bits
 *   - [sur 1 octets - quint8]  le type de paquet = 0 pour la bonne réception (1 seul type pour l'instant)
 *   - [sur 4 octets - quint32] la longueur de l'ID unique
 *   - [sur x octets - QString] l'ID unique sous forme de QString (le même que lors de l'envoi)
 *   - [sur 2 octets - quint16] 0xFFFF = fin de paquet
 */
class NetworkTransfert : public QObject
{
    Q_OBJECT

    private:
        /* La configuration */
        Configuration *m_config;

        /* Les écouteurs */
        QVector<NetworkTransfertListener *> *m_listeners;

        /* Le socket TCP */
        QTcpSocket m_tcpSocket;

        /* La taille du block à lire (0: inconnu, >0: nb octets à lire, 0xFFFF: plus de données à lire) */
        quint16 m_nextBlockSize;

        /* L'enregistreur de session duquel lire les réponses */
        RecorderSession *m_recorderSession;

        /* Indique si une erreur a déjà été reportée */
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

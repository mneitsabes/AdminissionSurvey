#ifndef RECORDERSESSION_H
#define RECORDERSESSION_H

#include <QVector>
#include <QCryptographicHash>
#include <QUuid>

#include "Record.h"
#include "IllegalArgumentException.h"
#include "Configuration.h"

/*
 * Enregistreur de réponses pour une session.
 *
 * Permet de sauver les réponses aux questions posées. Si une question est sautée, il n'y aura pas d'entrée pour cette dernière.
 * Le tout est associé au numéro d'admission du patient.
 */
class RecorderSession
{
    private:
        /* La configuration */
        Configuration *m_config;

        /* Les différentes enregistrements */
        QVector<Record *> *m_records;

        /* Le numéro d'admission du patient */
        QString m_numAdm;

        /* L'ID unique de session */
        QString m_uniqueId;

    public:
        RecorderSession(Configuration *config);
        ~RecorderSession();
        void addRecord(Record *record);
        QVector<Record *> * getRecords();
        QVector<Record *> * getRecordsCompletedAndOrdered();
        void setNumAdm(QString numAdm);
        QString getNumAdm();
        QString getUniqueId();

    private:
        bool isIdExists(int id);
        Record * getRecord(int id);
};

#endif // RECORDERSESSION_H

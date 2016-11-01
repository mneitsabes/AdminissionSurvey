#ifndef RECORDERSESSION_H
#define RECORDERSESSION_H

#include <QVector>
#include <QCryptographicHash>
#include <QUuid>

#include "Record.h"
#include "IllegalArgumentException.h"
#include "Configuration.h"

/*
 * Enregistreur de r�ponses pour une session.
 *
 * Permet de sauver les r�ponses aux questions pos�es. Si une question est saut�e, il n'y aura pas d'entr�e pour cette derni�re.
 * Le tout est associ� au num�ro d'admission du patient.
 */
class RecorderSession
{
    private:
        /* La configuration */
        Configuration *m_config;

        /* Les diff�rentes enregistrements */
        QVector<Record *> *m_records;

        /* Le num�ro d'admission du patient */
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

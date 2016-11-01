#ifndef RESPONSESSET_H
#define RESPONSESSET_H

#include <QString>
#include <QVector>

#include "QuestionResponse.h"

/*
 * Ensemble de réponses transmis par un client AdmissionSurvey.
 *
 * Cet ensemble est composé de :
 *    - un ID unique de session
 *    - Des réponses aux questions (voir QuestionResponse)
 *    - Un numéro d'admission
 */
class ResponsesSet
{
    private:
        /* ID unique de session */
        QString m_uniqeId;

        /* Les réponses aux questions */
        QVector<QuestionResponse> m_responses;

        /* Le numéro d'admission */
        QString m_numAdm;

    public:
        ResponsesSet();
        void setUniqueId(QString uniqueId);
        void addResponse(int questionId, QString responseValue);
        void setNumAdm(QString numAdm);
        QString getUniqueId();
        QVector<QuestionResponse> getResponses();
        QString getNumAdm();
};

#endif // RESPONSESSET_H

#ifndef RESPONSESSET_H
#define RESPONSESSET_H

#include <QString>
#include <QVector>

#include "QuestionResponse.h"

/*
 * Ensemble de r�ponses transmis par un client AdmissionSurvey.
 *
 * Cet ensemble est compos� de :
 *    - un ID unique de session
 *    - Des r�ponses aux questions (voir QuestionResponse)
 *    - Un num�ro d'admission
 */
class ResponsesSet
{
    private:
        /* ID unique de session */
        QString m_uniqeId;

        /* Les r�ponses aux questions */
        QVector<QuestionResponse> m_responses;

        /* Le num�ro d'admission */
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

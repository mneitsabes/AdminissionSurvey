#include "ResponsesSet.h"

/*
 * Constructeur.
 */
ResponsesSet::ResponsesSet() { }

/*
 * Permet de définir l'ID unique.
 *
 * @param uniqueId l'ID unique
 */
void ResponsesSet::setUniqueId(QString uniqueId)
{
    m_uniqeId = uniqueId;
}

/*
 * Ajoute une réponse à l'ensemble.
 *
 * Créé un QuestionResponse pour ajouter à l'ensemble.
 *
 * @param questionId l'ID de la question
 * @param responseValue la valeur de réponse
 */
void ResponsesSet::addResponse(int questionId, QString responseValue)
{
    //création de la structure
    QuestionResponse qr;
    qr.questionId = questionId;
    qr.responseValue = responseValue;

    //ajout de la réponse à l'ensemble
    m_responses.append(qr);
}

/*
 * Permet de définir le numéro d'admission.
 *
 * @param numAdm le numéro d'admission
 */
void ResponsesSet::setNumAdm(QString numAdm)
{
    m_numAdm = numAdm;
}

/*
 * Retourne l'ID unique.
 *
 * @return l'ID unique
 */
QString ResponsesSet::getUniqueId()
{
    return m_uniqeId;
}

/*
 * Permet de récupérer les réponses aux questions.
 *
 * @return les réponses aux questions
 */
QVector<QuestionResponse> ResponsesSet::getResponses()
{
    return m_responses;
}

/*
 * Retourne le numéro d'admission.
 *
 * @return le numéro d'admission
 */
QString ResponsesSet::getNumAdm()
{
    return m_numAdm;
}

#include "ResponsesSet.h"

/*
 * Constructeur.
 */
ResponsesSet::ResponsesSet() { }

/*
 * Permet de d�finir l'ID unique.
 *
 * @param uniqueId l'ID unique
 */
void ResponsesSet::setUniqueId(QString uniqueId)
{
    m_uniqeId = uniqueId;
}

/*
 * Ajoute une r�ponse � l'ensemble.
 *
 * Cr�� un QuestionResponse pour ajouter � l'ensemble.
 *
 * @param questionId l'ID de la question
 * @param responseValue la valeur de r�ponse
 */
void ResponsesSet::addResponse(int questionId, QString responseValue)
{
    //cr�ation de la structure
    QuestionResponse qr;
    qr.questionId = questionId;
    qr.responseValue = responseValue;

    //ajout de la r�ponse � l'ensemble
    m_responses.append(qr);
}

/*
 * Permet de d�finir le num�ro d'admission.
 *
 * @param numAdm le num�ro d'admission
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
 * Permet de r�cup�rer les r�ponses aux questions.
 *
 * @return les r�ponses aux questions
 */
QVector<QuestionResponse> ResponsesSet::getResponses()
{
    return m_responses;
}

/*
 * Retourne le num�ro d'admission.
 *
 * @return le num�ro d'admission
 */
QString ResponsesSet::getNumAdm()
{
    return m_numAdm;
}

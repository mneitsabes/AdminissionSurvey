#ifndef QUESTIONRESPONSE_H
#define QUESTIONRESPONSE_H

#include <QString>

/*
 * Structure représentant une réponse à une question.
 */
struct QuestionResponse
{
    /* L'ID de la question */
    int questionId;

    /* La valeur de réponse à la question */
    QString responseValue;
};

#endif // QUESTIONRESPONSE_H

#ifndef QUESTIONRESPONSE_H
#define QUESTIONRESPONSE_H

#include <QString>

/*
 * Structure repr�sentant une r�ponse � une question.
 */
struct QuestionResponse
{
    /* L'ID de la question */
    int questionId;

    /* La valeur de r�ponse � la question */
    QString responseValue;
};

#endif // QUESTIONRESPONSE_H

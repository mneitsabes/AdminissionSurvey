#ifndef RECORD_H
#define RECORD_H

#include <QString>

#include "Question.h"

/*
 * Représente un enregistrement d'une réponse pour une question donnée.
 */
class Record
{
    private:
        /* La question */
        Question *m_question;

        /* La valeur liée à la réponse choisie par l'utilisateur */
        QString m_value;

    public:
        Record(Question *question, QString value);
        Question * getQuestion();
        QString getValue();
};

#endif // RECORD_H

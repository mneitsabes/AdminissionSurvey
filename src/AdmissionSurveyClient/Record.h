#ifndef RECORD_H
#define RECORD_H

#include <QString>

#include "Question.h"

/*
 * Repr�sente un enregistrement d'une r�ponse pour une question donn�e.
 */
class Record
{
    private:
        /* La question */
        Question *m_question;

        /* La valeur li�e � la r�ponse choisie par l'utilisateur */
        QString m_value;

    public:
        Record(Question *question, QString value);
        Question * getQuestion();
        QString getValue();
};

#endif // RECORD_H

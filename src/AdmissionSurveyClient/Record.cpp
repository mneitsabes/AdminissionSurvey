#include "Record.h"

/*
 * Constructeur.
 *
 * @param question la question
 * @param value la valeur de la réponse choisie par l'utilisateur
 */
Record::Record(Question *question, QString value)
{
    m_question = question;
    m_value = value;
}

/*
 * Retourne la question.
 *
 * @return la question
 */
Question * Record::getQuestion()
{
    return m_question;
}

/*
 * Retourne la valeur de la réponse choisie par l'utilisateur.
 *
 * @return la valeur
 */
QString Record::getValue()
{
    return m_value;
}

#ifndef RESSOURCEFORMATEXCEPTION_H
#define RESSOURCEFORMATEXCEPTION_H

#include <QObject>
#include <QString>

#include "RessourceException.h"

/*
 * Permet de preciser le type d'erreurs du format qui a provoque l'erreur.
 */
enum RessourceFormatExceptionType
{
    /* Multiple définition d'un même noeud */
    MULTI_DEF,

    /* Format de la valeur incorrect */
    FORMAT_VALUE,

    /* Valeur impossible */
    BAD_VALUE,

    /* Noeud manquant */
    MISSING_TAG,

    /* Noeud inconnu */
    UNKNOWN_TAG,

    /* ID d'une question manquant */
    ID_MISSING,

    /* Attribut inconnu */
    UNKNOWN_ATTRIBUTE,

    /* Attribut manquant */
    MISSING_ATTRIBUTE
};

/*
 *  Exception lancée lorsqu'il y a un problème de format
 *
 *  Exception lancée lorsqu'il y a un problème de format (sémantique) dans le fichier XML.
 */
class RessourceFormatException : public RessourceException
{
    Q_OBJECT

    private:
        /* Le type d'erreurs de format */
        RessourceFormatExceptionType m_type;

        /* L'ID de la question */
        int m_questionId;

        /* Le tag s'il s'agit d'une erreur de base inconnue */
        QString m_tag;

    public:
        RessourceFormatException(QString tag, RessourceFormatExceptionType type) throw();
        ~RessourceFormatException() throw();
        void setQuestionId(int questionId);
        void setTag(QString tag);
        RessourceFormatExceptionType getType();
        int getQuestionId();
        QString getTag();
        QString getTitle();
        QString getMessage();
};

#endif // RESSOURCEFORMATEXCEPTION_H

#ifndef RESSOURCEEXCEPTION_H
#define RESSOURCEEXCEPTION_H

#include <stdexcept>

#include <QObject>
#include <QString>
#include <QVector>

using namespace std;

/*
 *  Classe mère des exceptions lancées lors de la création des questions.
 *
 *  Chaque exception doit fournir une chemin d'une icone, un titre et un message pour afficher l'erreur au responsable.
 */
class RessourceException : public exception, public QObject
{
    public:
        RessourceException() throw();
        ~RessourceException() throw();
        virtual QString getTitle() = 0;
        virtual QString getMessage() = 0;
};

#endif // RESSOURCEEXCEPTION_H

#ifndef ILLEGALARGUMENTEXCEPTION_H
#define ILLEGALARGUMENTEXCEPTION_H

#include <stdexcept>

#include <QObject>
#include <QString>

using namespace std;

/*
 *  Exception lancée lorsqu'une méthode recoit des paramètres incorrects ou incohérents vis-à-vis de l'état de l'objet.
 */
class IllegalArgumentException : public exception, public QObject
{
    private:
        /* Le message */
        QString m_message;

    public:
        IllegalArgumentException(QString message) throw();
        ~IllegalArgumentException() throw();
        QString getMessage();
        const char* what() const throw();
};

#endif // ILLEGALARGUMENTEXCEPTION_H

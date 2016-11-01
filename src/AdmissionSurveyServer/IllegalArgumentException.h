#ifndef ILLEGALARGUMENTEXCEPTION_H
#define ILLEGALARGUMENTEXCEPTION_H

#include <stdexcept>

#include <QObject>
#include <QString>

using namespace std;

/*
 *  Exception lanc�e lorsqu'une m�thode recoit des param�tres incorrects ou incoh�rents vis-�-vis de l'�tat de l'objet.
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

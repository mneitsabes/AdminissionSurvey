#include "IllegalArgumentException.h"

/*
 * Constructeur.
 */
IllegalArgumentException::IllegalArgumentException(QString message) throw() : QObject()
{
    m_message = message;
}

/*
 * Destructeur.
 */
IllegalArgumentException::~IllegalArgumentException() throw() {}

/*
 * Retourne le message de l'exception.
 *
 * @return le message de l'exception
 */
QString IllegalArgumentException::getMessage()
{
    return m_message;
}

/*
 * Retourne le message de l'exception.
 *
 * @return le message de l'exception
 */
const char* IllegalArgumentException::what() const throw()
{
    return m_message.toStdString().c_str();
}

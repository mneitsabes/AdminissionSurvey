#ifndef QUESTIONDATA_H
#define QUESTIONDATA_H

#include <QByteArray>
#include <QUrl>

/*
 * Repr�sente les donn�es d'une question, � savoir :
 *    - les donn�es HTML repr�santant la question
 *    - l'URL de la base permettant de charger correctement les ressources (images, css, javascript, ...) utilis�es dans les donn�es HTML
 */
class QuestionData
{
    private:
        /* Les donn�es HTML repr�sentant la question */
        QByteArray m_data;

        /* La base pour les ressources */
        QUrl m_baseUrl;

    public:
        QuestionData(QByteArray data, QUrl baseUrl);
        QByteArray getData();
        QUrl getBaseUrl();
};

#endif // QUESTIONDATA_H

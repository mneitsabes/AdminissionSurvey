#ifndef QUESTIONDATA_H
#define QUESTIONDATA_H

#include <QByteArray>
#include <QUrl>

/*
 * Représente les données d'une question, à savoir :
 *    - les données HTML représantant la question
 *    - l'URL de la base permettant de charger correctement les ressources (images, css, javascript, ...) utilisées dans les données HTML
 */
class QuestionData
{
    private:
        /* Les données HTML représentant la question */
        QByteArray m_data;

        /* La base pour les ressources */
        QUrl m_baseUrl;

    public:
        QuestionData(QByteArray data, QUrl baseUrl);
        QByteArray getData();
        QUrl getBaseUrl();
};

#endif // QUESTIONDATA_H

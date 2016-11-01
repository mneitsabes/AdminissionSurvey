#ifndef QUESTIONHTML_H
#define QUESTIONHTML_H

#include <QString>
#include <QMap>
#include <QDebug>

#include "Question.h"
#include "Response.h"
#include "Language.h"
#include "RessourceErrorException.h"
#include "Tools.h"
#include "IllegalArgumentException.h"

/*
 * Repr�sente une question du sondage.
 *
 * Une question est compos�e d'un ID unique, les diff�rentes r�ponses et d'un fichier par langue.
 * Le contenu de la question est stock� au format HTML dans un fichier.
 */
class Question
{
    private:
        /* L'id de la question */
        int m_id;

        /* Les r�ponses � la question */
        Response *m_response;

        /* Map : un fichier par langue */
        QMap<Language, QString> *m_files;

    public:
        Question(int id=0);
        ~Question();
        void addFile(Language lg, QString path);
        void setResponse(Response *response);
        QString getPath(Language lg);
        QByteArray getHTML(Language lg);
        int getId();
        Response * getResponse();
};

#endif // QUESTIONHTML_H

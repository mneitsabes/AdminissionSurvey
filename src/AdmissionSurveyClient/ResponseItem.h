#ifndef RESPONSEITEM_H
#define RESPONSEITEM_H

#include <QDebug>
#include <QString>
#include <QMap>

#include "Language.h"
#include "IllegalArgumentException.h"

/*
 * Les diff�rentes actions possibles lorsque l'on s�lectionne un �l�ment de r�ponse.
 */
enum ResponseItemAction {
    /* Passer � la question suivante (comportement par d�faut) */
    NEXT_QUESTION,

    /* Sauter jusqu'� la question sp�cifi�e par m_jumpToId ou terminer le sondage (si m_jumpToId = 0) */
    JUMP_TO,
};

/*
 * Repr�sente un �l�ment de r�ponse.
 *
 * Un �l�ment de r�ponse est compos� :
 *   - d'un tag : le tag est une cha�ne qui sera utilis� comme valeur d'un lien (<a href="<TAG>"...) permettant de lier un lien � l'�l�ment de r�ponse
 *   - d'un label : il s'agit du texte du bouton
 *   - d'une valeur : la valeur li�e � la r�ponse � sauvegard�e dans l'enregistreur de session
 *   - d'une action : lorsque l'on s�lecitonne cet �l�ment de r�ponse, quel doit �tre le comprotement du moteur
 *   - d'un ID de saut : si l'action est JUMP_TO, le moteur sautera � cet ID de question directement. Si la valeur est 0, il faut terminer le sondage
 */
class ResponseItem
{
    private:
        /* Le tag */
        QString m_tag;

        /* Les labels : un par langue */
        QMap<Language, QString> *m_labels;

        /* La valeur */
        QString m_value;

        /* L'action */
        ResponseItemAction m_action;

        /* L'ID de saut, si 0 : fin du questionnaire */
        int m_jumpToId;

    public:
        ResponseItem(QString tag="", QString value="");
        ~ResponseItem();
        void setResponseItemAction(ResponseItemAction action, int jumpToId=-1);
        void addLabel(Language lg, QString label);
        void setValue(QString value);
        QString getTag();
        QString getValue();
        QString getLabel(Language lg);
        ResponseItemAction getAction();
        int getJumpToId();
};

#endif // RESPONSEITEM_H

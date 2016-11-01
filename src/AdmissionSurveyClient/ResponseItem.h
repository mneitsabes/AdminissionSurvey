#ifndef RESPONSEITEM_H
#define RESPONSEITEM_H

#include <QDebug>
#include <QString>
#include <QMap>

#include "Language.h"
#include "IllegalArgumentException.h"

/*
 * Les différentes actions possibles lorsque l'on sélectionne un élément de réponse.
 */
enum ResponseItemAction {
    /* Passer à la question suivante (comportement par défaut) */
    NEXT_QUESTION,

    /* Sauter jusqu'à la question spécifiée par m_jumpToId ou terminer le sondage (si m_jumpToId = 0) */
    JUMP_TO,
};

/*
 * Représente un élément de réponse.
 *
 * Un élément de réponse est composé :
 *   - d'un tag : le tag est une chaîne qui sera utilisé comme valeur d'un lien (<a href="<TAG>"...) permettant de lier un lien à l'élément de réponse
 *   - d'un label : il s'agit du texte du bouton
 *   - d'une valeur : la valeur liée à la réponse à sauvegardée dans l'enregistreur de session
 *   - d'une action : lorsque l'on sélecitonne cet élément de réponse, quel doit être le comprotement du moteur
 *   - d'un ID de saut : si l'action est JUMP_TO, le moteur sautera à cet ID de question directement. Si la valeur est 0, il faut terminer le sondage
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

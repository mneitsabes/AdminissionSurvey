#include "ResponseItem.h"

/*
 * Créé un élément de réponse.
 *
 * L'action par défaut est de passer à la question suivante.
 *
 * @param tag le tag de l'élément de réponse
 * @param value la valeur de l'élément de réponse
 */
ResponseItem::ResponseItem(QString tag, QString value)
{
    m_labels = new QMap<Language, QString>();
    m_tag = tag;
    m_value = value;
    m_action = NEXT_QUESTION;
    m_jumpToId = -1;
}

/*
 * Destructeur.
 */
ResponseItem::~ResponseItem()
{
    m_labels->clear();
    delete m_labels;
    m_labels = NULL;
}

/*
 * Permet de définir une autre action que celle par défaut.
 *
 * Si l'action est JUMP_TO, un ID doit être défini (0 : fin du questionne, >0 : sauter à la question spécifiée)
 *
 * @param action l'action
 * @param jumpToId l'ID de la question suivante
 */
void ResponseItem::setResponseItemAction(ResponseItemAction action, int jumpToId)
{
    if(action == JUMP_TO && jumpToId == -1)
        throw new IllegalArgumentException("If action = JUMP_TO, the jump ID must be >= 0");

    m_action = action;
    m_jumpToId = jumpToId;
}

/*
 * Permet d'ajouter le label pour la langue spécifiée.
 *
 * @param lg la langue
 * @param label le label
 */
void ResponseItem::addLabel(Language lg, QString label)
{
    if(lg != NONE)
    {
        if(!m_labels->contains(lg))
            m_labels->insert(lg, label);
        else
            throw new IllegalArgumentException("Language already exists");
    }
    else
        throw new IllegalArgumentException("Language cannot be NONE");
}

/*
 * Permet de définir la valeur de l'élément de réponse.
 *
 * @param value la valeur de l'élément de réponse
 */
void ResponseItem::setValue(QString value)
{
    m_value = value;
}

/*
 * Retourne le tag de l'élément de réponse.
 *
 * @return le tag
 */
QString ResponseItem::getTag()
{
    return m_tag;
}

/*
 * Retourne la valeur de l'élément de réponse.
 *
 * @return la valeur
 */
QString ResponseItem::getValue()
{
    return m_value;
}

/*
 * Retourne l'action de l'élément de réponse.
 *
 * @return l'action
 */
ResponseItemAction ResponseItem::getAction()
{
    return m_action;
}

/*
 * Retourne l'ID de la question suivante dans le cas d'un saut de questions.
 *
 * @return l'ID
 */
int ResponseItem::getJumpToId()
{
    return m_jumpToId;
}

/*
 * Retourne le label de l'élément de réponse en fonction de la langue spécifiée.
 *
 * Interdit pour la langue "NONE".
 *
 * @return le label
 */
QString ResponseItem::getLabel(Language lg)
{
    if(lg != NONE)
        return m_labels->find(lg).value();
    else
        throw new IllegalArgumentException("Language cannot be NONE");
}

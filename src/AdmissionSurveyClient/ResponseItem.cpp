#include "ResponseItem.h"

/*
 * Cr�� un �l�ment de r�ponse.
 *
 * L'action par d�faut est de passer � la question suivante.
 *
 * @param tag le tag de l'�l�ment de r�ponse
 * @param value la valeur de l'�l�ment de r�ponse
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
 * Permet de d�finir une autre action que celle par d�faut.
 *
 * Si l'action est JUMP_TO, un ID doit �tre d�fini (0 : fin du questionne, >0 : sauter � la question sp�cifi�e)
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
 * Permet d'ajouter le label pour la langue sp�cifi�e.
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
 * Permet de d�finir la valeur de l'�l�ment de r�ponse.
 *
 * @param value la valeur de l'�l�ment de r�ponse
 */
void ResponseItem::setValue(QString value)
{
    m_value = value;
}

/*
 * Retourne le tag de l'�l�ment de r�ponse.
 *
 * @return le tag
 */
QString ResponseItem::getTag()
{
    return m_tag;
}

/*
 * Retourne la valeur de l'�l�ment de r�ponse.
 *
 * @return la valeur
 */
QString ResponseItem::getValue()
{
    return m_value;
}

/*
 * Retourne l'action de l'�l�ment de r�ponse.
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
 * Retourne le label de l'�l�ment de r�ponse en fonction de la langue sp�cifi�e.
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

#include "OutputItem.h"

OutputItem::OutputItem(QString filePath)
{
    if(filePath.isEmpty())
        throw new IllegalArgumentException("Filepath cannot be empty");

    m_filePath = filePath;
}

void OutputItem::addQuestionId(int questionId)
{
    if(m_questionIds.contains(questionId))
        throw new IllegalArgumentException(QString("Question ID (%1) already exists").arg(questionId));

    m_questionIds.append(questionId);
}

bool OutputItem::isQuestionIdAttached(int questionId)
{
    return m_questionIds.contains(questionId);
}

QString OutputItem::getFilePath()
{
    return m_filePath;
}

QVector<int> OutputItem::getQuestionsAttached()
{
    return m_questionIds;
}

int OutputItem::getNbQuestionsAttached()
{
    return m_questionIds.size();
}

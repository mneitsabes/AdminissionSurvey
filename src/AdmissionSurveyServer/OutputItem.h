#ifndef OUTPUTITEM_H
#define OUTPUTITEM_H

#include <QVector>
#include <QString>

#include "IllegalArgumentException.h"

class OutputItem
{
    private:
        QVector<int> m_questionIds;
        QString m_filePath;

    public:
        OutputItem(QString filePath);
        void addQuestionId(int questionId);
        bool isQuestionIdAttached(int questionId);
        QString getFilePath();
        QVector<int> getQuestionsAttached();
        int getNbQuestionsAttached();
};

#endif // OUTPUTITEM_H

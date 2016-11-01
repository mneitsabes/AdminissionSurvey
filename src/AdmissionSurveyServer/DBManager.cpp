#include "DBManager.h"

/*
 * Constructeur.
 *
 * @param path le chemin d'acc�s du fichier o� sont sauvegard�s les r�sultats
 */
DBManager::DBManager(Configuration *configuration, QString path)
{
    if(path.isEmpty())
        throw new IllegalArgumentException("Path cannot be empty");

    m_configuration = configuration;
    m_path = path;
}

/*
 * Permet de sauvegarder d'un ensemble de r�ponses dans le fichier.
 *
 * La structure est d�finie dans la documentation de cette classe.
 * Chaque �l�ment de sortie poss�de une ou plusieurs questions dont les r�ponses doivent y �tre sauvegard�es.
 *
 * @param responsesSet l'ensemble de r�ponses
 */
void DBManager::addResponsesSet(ResponsesSet responsesSet)
{
    QDateTime dateTime = QDateTime::currentDateTime();

    QVector<QuestionResponse> responses = responsesSet.getResponses();
    if(m_configuration->getNbQuestions() != responses.size())
    {
        std::cerr << "Error nb questions : " << m_configuration->getNbQuestions() << " - " << responses.size() << std::endl;
        return ;
    }

    //pour chaque �l�ments de sortie
    for(int i=0; i < m_configuration->getOutputItems()->size(); i++)
    {
        OutputItem *oi = m_configuration->getOutputItems()->at(i);

        //on construit une liste de r�ponses � partir des questions attach�es � l'�l�ment de sortie
        QVector<QuestionResponse> subResponses;
        for(int j=0; j < oi->getQuestionsAttached().size(); j++) //pour chaque question attach�e
        {
            //on l'ajoute � la liste des sous-r�ponses
            subResponses.append(
                    getQuestionResponseFromId(responses,
                                              oi->getQuestionsAttached().at(j)
                    )
            );
        }

        QString pathFile = m_path;
        pathFile = pathFile.append("/");
        pathFile = pathFile.append(oi->getFilePath());

        appendToFile(pathFile, responsesSet.getUniqueId(), dateTime, subResponses, responsesSet.getNumAdm());
    }
}

/*
 * Ajoute les donn�es � la fin du fichier sp�cifi� en param�tre.
 *
 * @param pathFile le fichier dans lequel on ajoute les r�sultats
 * @param uniqueId ID unique de session
 * @param dateTime la date et l'heure courante
 * @param responses les r�sultats � sauv�s
 * @param numAdm le num�ro d'admission
 */
void DBManager::appendToFile(QString pathFile, QString uniqueId, QDateTime dateTime, QVector<QuestionResponse> responses, QString numAdm)
{
    //cr�ation des donn�e
    QByteArray block;
    QTextStream stream(&block, QIODevice::WriteOnly);
    stream << uniqueId;
    stream << ";";
    stream << dateTime.toString("dd/MM/yyyy");
    stream << ";";
    stream << dateTime.toString("hh:mm");
    stream << ";";

    for(int i=0; i < responses.size(); i++)
    {
        stream << responses.at(i).responseValue;
        stream << ";";
    }

    stream << numAdm;
    stream << "\r\n";
    stream.flush();

    //on �crit � la fin du fichier
    QFile file(pathFile);
    file.open(QIODevice::Append);
    file.write(block);
    file.close();
}

/*
 * Retourne la r�ponse � une question pr�sente dans la liste fournie pour l'ID fourni.
 *
 * @param qrs la liste
 * @param ID l'ID
 * @return la r�ponse � une question
 */
QuestionResponse DBManager::getQuestionResponseFromId(QVector<QuestionResponse> qrs, int questionId)
{
    for(int i=0; i < qrs.size(); i++)
    {
        if(qrs.at(i).questionId == questionId)
            return qrs.at(i);
    }

    throw new IllegalArgumentException(QString("Question ID (%1) not found").arg(questionId));
}

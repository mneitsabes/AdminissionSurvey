#include "DBManager.h"

/*
 * Constructeur.
 *
 * @param path le chemin d'accès du fichier où sont sauvegardés les résultats
 */
DBManager::DBManager(Configuration *configuration, QString path)
{
    if(path.isEmpty())
        throw new IllegalArgumentException("Path cannot be empty");

    m_configuration = configuration;
    m_path = path;
}

/*
 * Permet de sauvegarder d'un ensemble de réponses dans le fichier.
 *
 * La structure est définie dans la documentation de cette classe.
 * Chaque élément de sortie possède une ou plusieurs questions dont les réponses doivent y être sauvegardées.
 *
 * @param responsesSet l'ensemble de réponses
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

    //pour chaque éléments de sortie
    for(int i=0; i < m_configuration->getOutputItems()->size(); i++)
    {
        OutputItem *oi = m_configuration->getOutputItems()->at(i);

        //on construit une liste de réponses à partir des questions attachées à l'élément de sortie
        QVector<QuestionResponse> subResponses;
        for(int j=0; j < oi->getQuestionsAttached().size(); j++) //pour chaque question attachée
        {
            //on l'ajoute à la liste des sous-réponses
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
 * Ajoute les données à la fin du fichier spécifié en paramètre.
 *
 * @param pathFile le fichier dans lequel on ajoute les résultats
 * @param uniqueId ID unique de session
 * @param dateTime la date et l'heure courante
 * @param responses les résultats à sauvés
 * @param numAdm le numéro d'admission
 */
void DBManager::appendToFile(QString pathFile, QString uniqueId, QDateTime dateTime, QVector<QuestionResponse> responses, QString numAdm)
{
    //création des donnée
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

    //on écrit à la fin du fichier
    QFile file(pathFile);
    file.open(QIODevice::Append);
    file.write(block);
    file.close();
}

/*
 * Retourne la réponse à une question présente dans la liste fournie pour l'ID fourni.
 *
 * @param qrs la liste
 * @param ID l'ID
 * @return la réponse à une question
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

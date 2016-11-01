#include "DBManager.h"

/*
 * Constructeur.
 *
 * @param config la configuration
 */
DBManager::DBManager(Configuration *config)
{
    m_config = config;
}

/*
 * Permet de sauvegarder les résultats d'une session dans un fichier local.
 *
 * Le type d'écriture définit le fichier de destination. La structure est définie dans la documentation de cette classe.
 *
 * @param writeType le type d'écriture
 * @param recorderSession l'enregistreur de session
 */
void DBManager::save(WriteType writeType, RecorderSession *recorderSession)
{
    QDateTime dateTime = QDateTime::currentDateTime();

    //création des données
    QByteArray block;
    QTextStream stream(&block, QIODevice::WriteOnly);
    stream << recorderSession->getUniqueId();
    stream << ";";
    stream << dateTime.toString("dd/MM/yyyy");
    stream << ";";
    stream << dateTime.toString("hh:mm");
    stream << ";";

    //pour chaque question
    QVector<Record *> *records = recorderSession->getRecordsCompletedAndOrdered();
    for(int i=0; i < records->size(); i++)
    {
        if(records->at(i) != NULL) //record trouvé pour la question
            stream << records->at(i)->getValue();
        else
            stream << "/";

        stream << ";";
    }

    stream << recorderSession->getNumAdm();
    stream << "\r\n";
    stream.flush();

    //écriture à la fin du fichier
    QString path = (writeType == NORMAL)?m_config->getDBManager().pathNormal:m_config->getDBManager().pathError;
    QFile file(path);
    file.open(QIODevice::Append);
    file.write(block);
    file.close();
}

#include "RecorderSession.h"

/*
 * Constructeur.
 *
 * Un ID unique est géré, il s'agit de la représentation hexadécimale du hash MD5 d'un Unique Universal IDentifier.
 *
 * @param config la configuration
 */
RecorderSession::RecorderSession(Configuration *config)
{
    m_config = config;
    m_records = new QVector<Record *>();

    //unique ID = MD5 d'un Unique Universal IDentifier
    m_uniqueId = QCryptographicHash::hash(QUuid::createUuid().toString().toAscii(), QCryptographicHash::Md5).toHex();
}

/*
 * Destructeur.
 */
RecorderSession::~RecorderSession()
{
    qDeleteAll(*m_records);
    m_records->clear();
    delete m_records;
    m_records = NULL;

    m_config = NULL; //pas de destruction ici
}

/*
 * Ajoute un enregistrement.
 *
 * Pour ajouter un enregistrement, il faut qu'il reste de la place et que l'ID de la question ne soit pas déjà présent.
 *
 * @param record l'enregistrement
 */
void RecorderSession::addRecord(Record *record)
{
    if(m_records->size() + 1 <= m_config->getQuestionsSize()) //vérification de la présence d'espace libre
    {
        if(!isIdExists(record->getQuestion()->getId())) //l'ID de la question est-il déjà présent ?
            m_records->append(record);
        else
            throw new IllegalArgumentException("Question ID already exists");
    }
    else
        throw new IllegalArgumentException("No free space");
}

/*
 * Vérifie l'existence d'un enregistrement pour l'ID de la question fourni.
 *
 * @param l'ID
 * @return vrai si l'ID existe, faux sinon
 */
bool RecorderSession::isIdExists(int id)
{
    return (getRecord(id) != NULL);
}

/*
 * Retourne les différents enregistrements.
 *
 * @return les enregistrements
 */
QVector<Record *> * RecorderSession::getRecords()
{
    return m_records;
}

/*
 * Retourne les différents enregistrements en comblant les trous et en classant les réponses par ordre croissant des ID des questions.
 *
 * Dans le cas où un utilisateur a répondu à la question 1, 2, 4 mais a sauté la question 3, le vecteur aura les informations suivants :
 *    [0] : Record Question ID 1
 *    [1] : Record Question ID 2
 *    [2] : NULL
 *    [3] : Record Question ID 3
 *
 * Les éléments du vecteur seront classés par ordre croissant des ID des questions.
 *
 * @return les différents enregistrements
 */
QVector<Record *> * RecorderSession::getRecordsCompletedAndOrdered()
{
    QVector<Record *> *records = new QVector<Record *>();
    for(int i=0; i < m_config->getQuestionsSize(); i++)
        records->append(getRecord(i+1)); //ID = i+1

    return records;
}

/*
 * Permet de définir le numéro d'admission.
 *
 * @param numAdm le numéro d'admission
 */
void RecorderSession::setNumAdm(QString numAdm)
{
    m_numAdm = numAdm;
}

/*
 * Retourne le numéro d'admission.
 *
 * @return le numéro d'admission
 */
QString RecorderSession::getNumAdm()
{
    return m_numAdm;
}

/*
 * Retourne le record enregistré lié à la question dont l'ID est fourni.
 *
 * @return le record
 */
Record * RecorderSession::getRecord(int id)
{
    for(int i=0; i < m_records->size(); i++)
    {
        if(m_records->at(i)->getQuestion()->getId() == id)
            return m_records->at(i);
    }

    return NULL;
}

/*
 * Retourne l'ID unique de la session.
 *
 * @return l'ID unique
 */
QString RecorderSession::getUniqueId()
{
    return m_uniqueId;
}


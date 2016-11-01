#include "RecorderSession.h"

/*
 * Constructeur.
 *
 * Un ID unique est g�r�, il s'agit de la repr�sentation hexad�cimale du hash MD5 d'un Unique Universal IDentifier.
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
 * Pour ajouter un enregistrement, il faut qu'il reste de la place et que l'ID de la question ne soit pas d�j� pr�sent.
 *
 * @param record l'enregistrement
 */
void RecorderSession::addRecord(Record *record)
{
    if(m_records->size() + 1 <= m_config->getQuestionsSize()) //v�rification de la pr�sence d'espace libre
    {
        if(!isIdExists(record->getQuestion()->getId())) //l'ID de la question est-il d�j� pr�sent ?
            m_records->append(record);
        else
            throw new IllegalArgumentException("Question ID already exists");
    }
    else
        throw new IllegalArgumentException("No free space");
}

/*
 * V�rifie l'existence d'un enregistrement pour l'ID de la question fourni.
 *
 * @param l'ID
 * @return vrai si l'ID existe, faux sinon
 */
bool RecorderSession::isIdExists(int id)
{
    return (getRecord(id) != NULL);
}

/*
 * Retourne les diff�rents enregistrements.
 *
 * @return les enregistrements
 */
QVector<Record *> * RecorderSession::getRecords()
{
    return m_records;
}

/*
 * Retourne les diff�rents enregistrements en comblant les trous et en classant les r�ponses par ordre croissant des ID des questions.
 *
 * Dans le cas o� un utilisateur a r�pondu � la question 1, 2, 4 mais a saut� la question 3, le vecteur aura les informations suivants :
 *    [0] : Record Question ID 1
 *    [1] : Record Question ID 2
 *    [2] : NULL
 *    [3] : Record Question ID 3
 *
 * Les �l�ments du vecteur seront class�s par ordre croissant des ID des questions.
 *
 * @return les diff�rents enregistrements
 */
QVector<Record *> * RecorderSession::getRecordsCompletedAndOrdered()
{
    QVector<Record *> *records = new QVector<Record *>();
    for(int i=0; i < m_config->getQuestionsSize(); i++)
        records->append(getRecord(i+1)); //ID = i+1

    return records;
}

/*
 * Permet de d�finir le num�ro d'admission.
 *
 * @param numAdm le num�ro d'admission
 */
void RecorderSession::setNumAdm(QString numAdm)
{
    m_numAdm = numAdm;
}

/*
 * Retourne le num�ro d'admission.
 *
 * @return le num�ro d'admission
 */
QString RecorderSession::getNumAdm()
{
    return m_numAdm;
}

/*
 * Retourne le record enregistr� li� � la question dont l'ID est fourni.
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


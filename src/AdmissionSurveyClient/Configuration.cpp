#include "Configuration.h"

/*
 * Initialisation du moteur à partir du fichier XML.
 *
 * @param pathXML le fichier XML servant à la base de la configuration.
 */
Configuration::Configuration(QString pathXML)
{
    m_questions = new QVector<Question *>();

    //initialise EngineXML qui peuplera la configuration une fois son traitement terminé
    EngineXML engineXML(pathXML);
    engineXML.populate(this);
}

/*
 * Destructeur.
 */
Configuration::~Configuration()
{
    qDeleteAll(*m_questions);
    m_questions->clear();
    delete m_questions;
    m_questions = NULL;
}

/*
 * Permet de définir la configuration du réseau.
 *
 * L'adresse du serveur est au format IPv4.
 *
 * @param addr adresse IPv4 du serveur
 * @param port le port du serveur
 */
void Configuration::setNetwork(QString addr, int port)
{
    QRegExp regEx("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
    if(!regEx.exactMatch(addr))
        throw new IllegalArgumentException(QString("The IP address (%1) is not valid").arg(addr));

    if(port < 1 || port > 65535)
        throw new IllegalArgumentException(QString("Port (%1) is not in the valid range [1-65535]").arg(port));

    m_network.serverAddr = addr;
    m_network.serverPort = port;
}

/*
 * Permet de définir la configuration du GUI.
 *
 * @param fontFamily la police utilisée dans le fichier HTML
 * @param borderSize la taille des bordures au format #RRGGBB
 * @param borderColor la couleur des bordures
 */
void Configuration::setGUI(QString fontFamily, int borderSize, QString borderColor)
{
    if(borderSize < 0)
        throw new IllegalArgumentException(QString("Bordersize (%1) is not valid").arg(borderSize));

    QColor color(borderColor);
    if(!color.isValid())
        throw new IllegalArgumentException(QString("Bordercolor (%1) is not valid").arg(borderColor));

    m_gui.fontFamily = fontFamily;
    m_gui.borderSize = borderSize;
    m_gui.borderColor = color;
}

/*
 * Permet de définir la configuration du DBManager.
 *
 * @param pathNormal le fichier d'enregistrement lorsque l'envoi au serveur s'est bien passé
 * @param pathError le fichier d'enregistrement lorsque l'envoi au serveur a échoué
 */
void Configuration::setDBManager(QString pathNormal, QString pathError)
{
    if(pathNormal.isEmpty())
        throw new IllegalArgumentException("Pathnormal cannot be empty");

    if(pathError.isEmpty())
        throw new IllegalArgumentException("Patherror cannot be empty");

    m_dbManager.pathNormal = pathNormal;
    m_dbManager.pathError = pathError;
}

/*
 * Permet de définir le mode de fonctionnement comme "en développement".
 *
 * Ce mode rend l'application fenêtrée et ne supprime pas le curseur.
 *
 * @param devMode vrai pour le mode développement, faux sinon
 */
void Configuration::setDevMode(bool devMode)
{
    m_devMode = devMode;
}

/*
 * Retourne le nombre de questions.
 *
 * @return le nombre de questions
 */
int Configuration::getQuestionsSize()
{
    return m_questions->size();
}

/*
 * Retourne la configuration réseau.
 *
 * @return la configuration réseau
 */
ConfigurationNetwork Configuration::getNetwork()
{
    return m_network;
}

/*
 * Retourne la configuration GUI.
 *
 * @return la configuration GUI
 */
ConfigurationGUI Configuration::getGUI()
{
    return m_gui;
}

/*
 * Retourne la configuration DBManager.
 *
 * @return la configuration DBManager
 */
ConfigurationDBManager Configuration::getDBManager()
{
    return m_dbManager;
}

/*
 * Indique si l'on tourne en mode "développement".
 *
 * @return vrai si l'on est dans ce mode, faux sinon
 */
bool Configuration::isDevMode()
{
    return m_devMode;
}

/*
 * Initialisation du moteur à partir du moteur XML.
 *
 * Une fois les questions ajoutées, controle des IDs :
 *   - les IDs doivent se suivre
 *   - les IDs des sauts doivent être valides
 */
void Configuration::setQuestions(QVector<Question *> *questions)
{
    for(int i=0; i < questions->size(); i++)
        addQuestion(questions->at(i));

    //vérification des IDs
    int nbQuestions = questions->size();
    for(int trackingId=1; trackingId <= nbQuestions; trackingId++)
    {
        if(!isIdExists(trackingId))
            throw new RessourceFormatException(QString::number(trackingId), ID_MISSING);
    }

    //vérification de la validité des "jumpId"
    for(int currentQuestionId = 1; currentQuestionId < nbQuestions; currentQuestionId++)
    {
        Question *question = getQuestion(currentQuestionId);

        QVector<ResponseItem *> responseItems = question->getResponse()->getAllResponseItem();
        for(int i=0; i < responseItems.size(); i++)
        {
            ResponseItem *ri = responseItems.at(i);
            if(ri->getAction() == JUMP_TO)
            {
                if(ri->getJumpToId() != 0 && (ri->getJumpToId() <= currentQuestionId || ri->getJumpToId() > nbQuestions))
                    throw new IllegalArgumentException(QString("Question %1 has an incorrect JumpToId").arg(currentQuestionId));
            }
        }
    }
}

/*
 * Ajoute une question à la liste des questions.
 *
 * @param question la question
 */
void Configuration::addQuestion(Question *question)
{
    if(!isIdExists(question->getId())) //ajouter que si la question n'est pas déjà présente
        m_questions->append(question);
    else
        throw new IllegalArgumentException("Question already exists");
}

/*
 * Vérifie l'existence d'une question dont l'ID est fourni.
 *
 * @param id l'id
 * @return vrai si l'ID existe, faux sinon
 */
bool Configuration::isIdExists(int id)
{
    for(int i=0; i < m_questions->size(); i++)
    {
        if(m_questions->at(i)->getId() == id)
            return true;
    }

    return false;
}

/*
 * Retourne la question dont l'ID est fourni.
 *
 * @param id l'ID
 * @return la question
 */
Question * Configuration::getQuestion(int id)
{
    for(int i=0; i < m_questions->size(); i++)
    {
        if(m_questions->at(i)->getId() == id)
            return m_questions->at(i);
    }

    throw new IllegalArgumentException("Question ID not found");
}


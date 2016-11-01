#include "Engine.h"

/*
 * Construit le moteur et sa configuration à partir d'un fichier XML.
 *
 * @param pathXML la localisation du fichier XML
 */
Engine::Engine(QString pathXML)
{
    m_listeners = new QVector<EngineListener *>();
    m_recorderSession = NULL;
    m_currentQuestion = NULL;
    m_currentLg = NONE;

  // m_sound = new QSound(":/patient_risk.wav");

    //initialisation à partir du fichier XML
    m_config = new Configuration(pathXML);

    //création et écoute du modèle de transfert
    m_neworkTransfert = new NetworkTransfert(m_config);
    m_neworkTransfert->addNetworkTransfertListener(this);

    //gestionnaire de sauvegardes locales
    m_dbManager = new DBManager(m_config);

    //on démarre la première session du programme
    newSession();
}

/*
 * Destructeur.
 */
Engine::~Engine()
{
    m_currentQuestion = NULL;

    m_listeners->clear(); //ne détruit pas les écouteurs ici
    delete m_listeners;
    m_listeners = NULL;

    delete m_neworkTransfert;
    m_neworkTransfert = NULL;

    delete m_dbManager;
    m_dbManager = NULL;

    delete m_recorderSession;
    m_recorderSession = NULL;

    delete m_config;
    m_config = NULL;

    //delete m_sound;
    //m_sound = NULL;
}

/*
 * Ajout d'un écouteur.
 *
 * @param listener l'écouteur
 */
void Engine::addEngineListener(EngineListener *listener)
{
    if(!m_listeners->contains(listener)) //ajouter que si l'écouteur n'écoute pas déjà
        m_listeners->append(listener);
}

/*
 * Supprime un écouteur.
 *
 * @param listener l'écouteur
 */
void Engine::removeEngineListener(EngineListener *listener)
{
    int pos = m_listeners->indexOf(listener);
    if(pos != -1)
        m_listeners->remove(pos);
}

/*
 * Notifie les écouteurs d'un changement dans le moteur.
 */
void Engine::notifyListeners()
{
    for(int i=0; i < m_listeners->size(); i++)
        m_listeners->at(i)->engineStateChanged();
}

/*
 * Retourne la question courante.
 *
 * @return la question courante
 */
Question * Engine::getCurrentQuestion()
{
    return m_currentQuestion;
}

/*
 * Démarre une nouvelle session.
 *
 * Il s'agit de recommencer le processus de sondage pour un nouveau utilisateur.
 */
void Engine::newSession()
{
    //nouveau enregistrer de questions
    if(m_recorderSession != NULL)
        delete m_recorderSession;

    m_recorderSession = new RecorderSession(m_config);

    //remise de l'état initial du moteur
    m_engineStep = CHOOSE_LANGUAGE;
    m_currentQuestion = NULL;
    m_currentLg = NONE;

    notifyListeners();
}

/*
 * Retourne les données représentant la question courante.
 *
 * Les données représentant une question sont contenues dans un QuestionData, à savoir :
 *    - les données HTML représantant la question
 *    - l'URL de la base permettant de charger correctement les ressources (images, css, javascript, ...) utilisées dans les données HTML
 *
 * @return les données
 */
QuestionData Engine::getCurrentQuestionData()
{
    QByteArray data;
    QUrl baseUrl;

    switch(m_engineStep)
    {
        case CHOOSE_LANGUAGE:
            //le choix de la langue se fait à partir d'un seul fichier
            data = Tools::readAllFile(ENGINE_CHOOSELANGUAGE_FILE);
            baseUrl = QDir::current().absoluteFilePath(ENGINE_CHOOSELANGUAGE_FILE);
        break;
        case SURVEY:
        {
            //données de début de page
            data = getDataTopPage();

            //récupération des données HTML de la question courante
            data = data.append(m_currentQuestion->getHTML(m_currentLg));

            //concaténation des données HTML des réponses sauf si le type est "EMBEDED", auquel cas les réponses sont déjà dans les données HTML
            Response *response = m_currentQuestion->getResponse();
            if(response->getType() == YES_NO_UNKNOWN || response->getType() == YES_NO || response->getType() == DECLARED)
                data = data.append(response->getHTMLData(m_currentLg));

            //données de fin de page
            data = data.append(getDataBottomPage());

            baseUrl = QDir::current().absoluteFilePath(m_currentQuestion->getPath(FR));
        }
        break;
        case END:
            //message de fin différent en fonction de la langue choisie
            if(m_currentLg == NL)
            {
                data = Tools::readAllFile(ENGINE_ENDMESSAGE_NL_FILE);
                baseUrl = QDir::current().absoluteFilePath(ENGINE_ENDMESSAGE_NL_FILE);
            }
            else if(m_currentLg == EN)
            {
                data = Tools::readAllFile(ENGINE_ENDMESSAGE_EN_FILE);
                baseUrl = QDir::current().absoluteFilePath(ENGINE_ENDMESSAGE_EN_FILE);
            }
            else
            {
                data = Tools::readAllFile(ENGINE_ENDMESSAGE_FR_FILE);
                baseUrl = QDir::current().absoluteFilePath(ENGINE_ENDMESSAGE_FR_FILE);
            }
        break;
        case ENTER_NUM_ADM:
            //l'encodage du numéro d'admission se fait toujours en français
            data = Tools::readAllFile(ENGINE_ASKNUMADM_FILE);
            baseUrl = QDir::current().absoluteFilePath(ENGINE_ASKNUMADM_FILE);
        break;
        default: ; //suppress warnings
    }

    return QuestionData(data, baseUrl);
}

/*
 * Préviens le moteur qu'un lien a été cliqué dans la vue HTML.
 *
 * En fonction de l'OS, l'URL est complète (file://<path>/<HTML_FILE>/<HREF_CLICKED>) ou non (<HREF_CLICKED>).
 * L'URL est nettoyée pour ne garder que la fin et le traitement est passé à elementClicked(QString).
 * Les caractères de la valeur sont ensuite passés en majuscules.
 *
 * @param url l'URL du lien cliqué
 */
void Engine::elementClicked(QUrl url)
{
    QString value = url.toString();
    if(value.lastIndexOf("/") != -1) //ne garde que la fin de l'URL (<HREF_CLICKED>)
        value = value.right(value.length()-value.lastIndexOf("/")-1);

    //mise en majuscule (important sous linux)
    value = value.toUpper();

    transfertUserData(value);
}

/*
 * Transfert de données au moteur à partir des actions de l'utilisateur.
 *
 * Est, par exemple, utilisée pour transmettre le choix d'une réponse à une question.
 * Cette méthode permet de faire évoluer l'état du moteur à partir des actions de l'utilisateur.
 * Elle traite :
 *    - le choix de la langue
 *    - le choix des réponses aux questions
 *    - l'encodage du numéro d'admission
 *
 * A noter que setCurrentQuestion(Question) se charge de prévenir les écouteurs d'un changement d'état.
 *
 * @param value la valeur du lien cliqué
 */
void Engine::transfertUserData(QString value)
{
    switch(m_engineStep)
    {
        case CHOOSE_LANGUAGE:
            //choix de la langue
            if(value == "CHOOSE_LG:FR" || value == "CHOOSE_LG:NL" || value == "CHOOSE_LG:EN")
            {
                //le choix de l'utilisateur est sauvé pour toute la session
                if(value == "CHOOSE_LG:FR")      m_currentLg = FR;
                else if(value == "CHOOSE_LG:NL") m_currentLg = NL;
                else if(value == "CHOOSE_LG:EN") m_currentLg = EN;

                //on démarre la partie sondage à la première question
                m_engineStep = SURVEY;
                setCurrentQuestion(m_config->getQuestion(1));
            }
        break;
        case SURVEY:
        {
            //on récupère l'élément de réponse correspondant à la valeur transmise
            Response *response = m_currentQuestion->getResponse();
            ResponseItem *ri = response->getResponseItem(value);

            //si l'on trouve une correspondance, c'est que l'utilisateur a cliqué sur une réponse
            if(ri != NULL)
            {
                //création d'un enregistrement pour la question et sauvegarde dans l'enregistreur de session
                Record *record = new Record(m_currentQuestion, ri->getValue());
                m_recorderSession->addRecord(record);

                //détermination de la question suivant
                int nextQuestionId;

                if(ri->getAction() == JUMP_TO) //si la réponse demande de sauter à une question spécifique plus lointaine
                    nextQuestionId = ri->getJumpToId();
                else //sinon le moteur passe à la question suivante
                    nextQuestionId = m_currentQuestion->getId() + 1;

                if(nextQuestionId != 0 && nextQuestionId <= m_config->getQuestionsSize()) //si l'on n'a pas encore atteind la fin, on affiche la question suivante
                    setCurrentQuestion(m_config->getQuestion(nextQuestionId));
                else //sinon le message de fin
                {
                    m_engineStep = END;
                    notifyListeners();
                }
            }
        }
        break;
        case ENTER_NUM_ADM:
            if(value == "RESET_SURVEY") //on ne sauve rien, on lance une nouvelle session
                newSession();
            else if(value.startsWith("SUBMIT_NUMADM:"))
            {
                //le numéro d'admission est transmis sous le format "SUBMIT_NUMADM:XXXXXXX" où les X représentent le numéro d'admission de 7 chiffres
                value = value.right(value.length() - value.indexOf(":") - 1);

                //on enregistre le numéro d'admission
                m_recorderSession->setNumAdm(value);

                //nous sommes à la fin du sondage nécessitant la sauvegarde et l'envoi des résultats au serveur
                m_engineStep = SAVING_RECORDS;
                saveRemote();
            }
        break;
        default: ; //enlève les warnings à la compilation
    }
}

/*
 * Préviens le moteur qu'une touche a été pressée dans la vue.
 *
 * Est utilisée pour détecter la touche "ENTER" lorsque l'état du moteur est en "END" (fin des questions).
 * Cette touche est pressée par le responsable du sondage pour passer à l'étape d'encodage du numéro d'admission.
 *
 * @param key keycode de la touche pressée
 */
void Engine::keyPressed(int key)
{
    if(m_engineStep == END)
    {
        if(key == Qt::Key_Enter || m_config->isDevMode())
        {
            //le responsable du sondage souhaite encoder le numéro d'admission
            m_engineStep = ENTER_NUM_ADM;
            notifyListeners();
        }
    }
}

/*
 * Permet de définir la question courante.
 *
 * Cette méthode s'occupe de prévenir les écouteurs du changement de question.
 *
 * @param question la nouvelle question
 */
void Engine::setCurrentQuestion(Question *question)
{
    m_currentQuestion = question;
    notifyListeners();
}

/*
 * Transfère les résultats au serveur pour réaliser une sauvegarde distante.
 */
void Engine::saveRemote()
{
    m_neworkTransfert->sendRecords(m_recorderSession);
}

/*
 * Invoqué sur la sauvegarde distante est réussie.
 *
 * Une sauvegarde locale est exécutée.
 * Une nouvelle session est lancée.
 *
 * @param uniqueId l'ID unique de l'enregistrement
 */
void Engine::recordTransfertSuccessful(QString uniqueId)
{
    std::cout << uniqueId.toStdString() << " : [REMOTE SAVED]" <<std::endl;

    m_dbManager->save(NORMAL, m_recorderSession);

    newSession();
}

/*
 * Invoqué sur la sauvegarde distante a échoué.
 *
 * Une sauvegarde locale "erreur" est exécutée.
 * Une nouvelle session est lancée et un message d'erreur est affiché.
 *
 * @param uniqueId l'ID unique de l'enregistrement
 * @param errorMsg le message d'erreur.
 */
void Engine::recordTransfertError(QString uniqueId, QString errorMsg)
{
    std::cerr << uniqueId.toStdString() << " : [ERROR] " << errorMsg.toStdString() <<std::endl;

    m_dbManager->save(ERROR, m_recorderSession);

    newSession();
}

/*
 * Retourne la progression du sondage sous forme de pourcentage.
 *
 * @return le pourcentage de la progression
 */
int Engine::getPercentProgress()
{
    int percentProgress = 0;

    switch(m_engineStep)
    {
        case CHOOSE_LANGUAGE:
            percentProgress = 0;
        break;
        case SURVEY:
            percentProgress = (int)((m_currentQuestion->getId() / (double)(m_config->getQuestionsSize()+1)) * 100);
        break;
        case END:
        case ENTER_NUM_ADM:
        case SAVING_RECORDS:
            percentProgress = 100;
    }

    return percentProgress;
}

/*
 * Retourne les données HTML du haut de la page.
 *
 * Inclusion de JQuery et de la barre de progression.
 *
 * @return données HTML
 */
QByteArray Engine::getDataTopPage()
{
    QString str = "<script type=\"text/javascript\" src=\"qrc:/jquery.js\"></script>";
    str = str.append("<script type=\"text/javascript\" src=\"qrc:/buttonSelected.js\"></script>");
    str = str.append("<link rel=\"stylesheet\" type=\"text/css\" href=\"qrc:/style_default.css\" />");
    str = str.append("<link rel=\"stylesheet\" type=\"text/css\" href=\"qrc:/style_button.css\" />");
    str = str.append("<div id=\"progress-outer\"><div id=\"progress-inner\"></div></div>");

    return str.toAscii();
}

/*
 * Retourne les données HTML de la fin de page.
 *
 * Définition de la taille de la barre de progression.
 *
 * @return données HTML
 */
QByteArray Engine::getDataBottomPage()
{
    QString str = QString("<script type=\"text/javascript\">$(document).ready(function() {  $(\"#progress-inner\").width(\"%1%\"); });</script>").arg(getPercentProgress());
    return str.toAscii();
}

/*
 * Retourne la configuration du moteur.
 *
 * @param la configuration
 */
Configuration * Engine::getConfiguration()
{
    return m_config;
}

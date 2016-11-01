#include "EngineXML.h"

/*
 * Constructeur.
 *
 * Ouvre, parse le document XML fourni et génère les questions.
 */
EngineXML::EngineXML(QString path)
{
    QFile file(path);
    if(file.exists())
    {
        QString errorStr;
        int errorLine;
        int errorColumn;

        //verifie les erreurs de synthaxe
        QDomDocument doc;
        if(!doc.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
            throw new RessourceParsingException(errorLine);

        //on cree les vecteurs
        m_questions = new QVector<Question *>();
        m_error = 0;

        try {
            //on traite le document
            parse(&doc);
        } catch(RessourceException *exception) {
            //on detruit les vecteurs alloués
            qDeleteAll(*m_questions);

            delete m_questions;
            m_questions = NULL;

            throw exception;
        }
    }
    else
        throw new RessourceOpenException(path);
}

/*
 * Destructeur.
 */
EngineXML::~EngineXML()
{
    m_questions->clear(); //ne détruit pas les questions ici
    delete m_questions;
    m_questions = NULL;
}

/*
 * Parse le fichier.
 *
 * S'occupe du niveau <survey> et <question> (passe le relaye pour ce noeud).
 *
 * @param doc le document XML
 */
void EngineXML::parse(QDomDocument *doc)
{
    //doit démarrer par le noeud racine "<survey>"
    QDomElement root = doc->documentElement();
    if(root.tagName() != "survey")
        throw new RessourceParsingException(-1);

    //on parcourt les noeuds
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        QString tagName = node.toElement().tagName();
        if(tagName == "general") //configuration générale
            parseGeneral(node);
        else if(tagName == "questions") //les questions
            parseQuestions(node);
        else
            throw new RessourceFormatException(tagName, UNKNOWN_TAG);

        //on passe au noeud suivant
        node = node.nextSibling();
    }
}

/*
 * Parse les noeuds de "<general>".
 *
 * @param node le noeud
 */
void EngineXML::parseGeneral(QDomNode node)
{
    //est-on en mode développement ?
    m_devMode = !node.toElement().attribute("devmode").isEmpty();

    //valeurs par défaut permettant de détecter l'absence de définition d'un noeud/attribut
    m_borderSize = -1;
    m_serverPort = -1;

    //on parcourt les noeuds
    node = node.firstChild();
    while(!node.isNull())
    {
        QString tagName = node.toElement().tagName();
        if(tagName == "font") //le noeud "font"
        {
            m_fontFamily = node.toElement().attribute("family");
            if(m_fontFamily.isEmpty())
                throw new RessourceFormatException("font>family", MISSING_ATTRIBUTE);
        }
        else if(tagName == "server")
        {
            m_serverAddr = node.toElement().attribute("addr");
            if(m_serverAddr.isEmpty())
                throw new RessourceFormatException("server>addr", MISSING_ATTRIBUTE);

            bool convertOk;
            m_serverPort = node.toElement().attribute("port").toInt(&convertOk, 10);
            if(!convertOk)
                throw new RessourceFormatException("server>port", BAD_VALUE);
        }
        else if(tagName == "gui")
        {
            bool convertOk;
            m_borderSize = node.toElement().attribute("bordersize").toInt(&convertOk, 10);
            if(!convertOk)
                throw new RessourceFormatException("gui>bordersize", BAD_VALUE);

            m_borderColor = node.toElement().attribute("bordercolor");
            if(m_borderColor.isEmpty())
                throw new RessourceFormatException("gui>bordercolor", MISSING_ATTRIBUTE);
        }
        else if(tagName == "path")
        {
            m_pathNormal = node.toElement().attribute("normal");
            if(m_pathNormal.isEmpty())
                throw new RessourceFormatException("path>normal", MISSING_ATTRIBUTE);

            m_pathError = node.toElement().attribute("error");
            if(m_pathError.isEmpty())
                throw new RessourceFormatException("path>error", MISSING_ATTRIBUTE);
        }
        else
            throw new RessourceFormatException(tagName, UNKNOWN_TAG);

        //on passe au noeud suivant
        node = node.nextSibling();
    }

    QString missingTag;
    if(m_fontFamily.isEmpty())         missingTag = "font>family";
    else if(m_borderSize == -1)        missingTag = "gui>bordersize";
    else if(m_borderColor.isEmpty())   missingTag = "gui>bordercolor";
    else if(m_serverAddr.isEmpty())    missingTag = "server>addr";
    else if(m_serverPort == -1)        missingTag = "server>port";
    else if(m_pathNormal.isEmpty())    missingTag = "path>normal";
    else if(m_pathError.isEmpty())     missingTag = "path>error";

    if(!missingTag.isEmpty())
        throw new RessourceFormatException(missingTag, MISSING_TAG);
}

/*
 * Parse les noeuds (normalement qu'un) "<questions">.
 *
 * @param questionsNode le noeud
 */
void EngineXML::parseQuestions(QDomNode questionsNode)
{
    //on parcourt les noeuds "question"
    QDomNode questionNode = questionsNode.firstChild();
    while(!questionNode.isNull())
    {
        QString tagName = questionNode.toElement().tagName();
        if(tagName == "question") //les questions
            parseQuestion(questionNode);
        else
            throw new RessourceFormatException(tagName, UNKNOWN_TAG);

        //on passe au noeud suivant
        questionNode = questionNode.nextSibling();
    }
}

/*
 * Parse les noeuds "<question>".
 *
 * Créé les réponses et éventuellement les éléments de réponses en fonction du noeud "<responses>".
 *
 * @param questioNode le noeud de la question
 */
void EngineXML::parseQuestion(QDomNode questionNode)
{
    //on récupère l'ID de la question
    bool convertOk;
    int questionId = questionNode.toElement().attribute("id").toInt(&convertOk, 10);
    if(!convertOk)
        throw new RessourceFormatException("id", BAD_VALUE);

    //création de la question
    Question *question = new Question(questionId);

    bool frTag = false;
    bool nlTag = false;
    bool enTag = false;
    bool responsesTag = false;

    //on parcourt le noeud de la question
    QDomNode node = questionNode.firstChild();
    while(!node.isNull())
    {
        QString subTagName = node.toElement().tagName();
        if(subTagName == "fr" || subTagName == "nl" || subTagName == "en") //si noeud <fr>, <nl>, <en>
        {
            //on récupère le chemin du fichier
            QString path = node.toElement().attribute("path");
            if(path.isEmpty())
                throw new RessourceErrorException(HTML_FILE, questionId);

            QFile file(path);
            if(file.exists())
            {
                if(subTagName == "fr")
                {
                    question->addFile(FR, path);
                    frTag = true;
                }
                else if(subTagName == "nl")
                {                  
                    question->addFile(NL, path);
                    nlTag = true;
                }
                else if(subTagName == "en")
                {
                    question->addFile(EN, path);
                    enTag = true;
                }
            }
            else
                throw new RessourceErrorException(HTML_FILE, questionId);
        }
        else if(subTagName == "responses") //si noeud "<responses>"
        {
            //en fonction du type
            QString responsesType = node.toElement().attribute("type");
            if(responsesType == "yes_no_unknown")
            {
                ResponseYesNoUnknown *response = new ResponseYesNoUnknown();

                //seul ces trois tags là peuvent être rédifinis via les noeuds "<response>"
                bool yesTag = false;
                bool noTag = false;
                bool unknownTag = false;

                QDomNode responseNode = node.firstChild();
                while(!responseNode.isNull())
                {
                    QString nodeTag = responseNode.toElement().tagName();
                    if(nodeTag == "response")
                    {
                        QString responseTag = responseNode.toElement().attribute("tag");
                        if(responseTag == "")
                            throw new RessourceFormatException(nodeTag, MISSING_ATTRIBUTE);

                        if(responseTag == RESPONSEYESNOUNKNOWN_TAG_YES)
                        {
                            if(yesTag)
                                throw new RessourceFormatException(responseTag, MULTI_DEF);

                            yesTag = true;
                        }
                        else if(responseTag == RESPONSEYESNOUNKNOWN_TAG_NO)
                        {
                            if(noTag)
                                throw new RessourceFormatException(responseTag, MULTI_DEF);

                            noTag = true;
                        }
                        else if(responseTag == RESPONSEYESNOUNKNOWN_TAG_UNKNOWN)
                        {
                            if(unknownTag)
                                throw new RessourceFormatException(responseTag, MULTI_DEF);

                            unknownTag = true;
                        }
                        else
                            throw new RessourceFormatException(responseTag, UNKNOWN_TAG);

                        //on chercher si la valeur ou l'ID de saut est défini
                        QString responseValue = responseNode.toElement().attribute("value");
                        QString responseJumpTo = responseNode.toElement().attribute("jumpto");

                        //si nécessaire, on modifie l'élément de réponse associé au tag
                        ResponseItem *ri = ((Response *)response)->getResponseItem(responseTag);

                        if(responseValue != "")
                            ri->setValue(responseValue);

                        if(responseJumpTo.toUpper() == "END") //fin du sondage
                            ri->setResponseItemAction(JUMP_TO, 0);
                        else if(responseJumpTo != "")
                        {
                            bool convertOk;
                            int jumpToId = responseJumpTo.toInt(&convertOk, 10);

                            if(!convertOk)
                                throw new RessourceFormatException("jumpto", BAD_VALUE);

                            ri->setResponseItemAction(JUMP_TO, jumpToId);
                        }

                        responseNode = responseNode.nextSibling();
                    }
                    else
                        throw new RessourceFormatException(nodeTag, UNKNOWN_TAG);
                }

                question->setResponse(response);
            }
            else if(responsesType == "yes_no")
            {
                ResponseYesNo *response = new ResponseYesNo();

                //seul ces deux tags là peuvent être rédifinis via les noeuds "<response>"
                bool yesTag = false;
                bool noTag = false;

                QDomNode responseNode = node.firstChild();
                while(!responseNode.isNull())
                {
                    QString nodeTag = responseNode.toElement().tagName();
                    if(nodeTag == "response")
                    {
                        QString responseTag = responseNode.toElement().attribute("tag");
                        if(responseTag == "")
                            throw new RessourceFormatException(nodeTag, MISSING_ATTRIBUTE);

                        if(responseTag == RESPONSEYESNO_TAG_YES)
                        {
                            if(yesTag)
                                throw new RessourceFormatException(responseTag, MULTI_DEF);

                            yesTag = true;
                        }
                        else if(responseTag == RESPONSEYESNO_TAG_NO)
                        {
                            if(noTag)
                                throw new RessourceFormatException(responseTag, MULTI_DEF);

                            noTag = true;
                        }
                        else
                            throw new RessourceFormatException(responseTag, UNKNOWN_TAG);

                        //on cherche si la valeur ou l'ID de saut est défini
                        QString responseValue = responseNode.toElement().attribute("value");
                        QString responseJumpTo = responseNode.toElement().attribute("jumpto");

                        //si nécessaire, on modifie l'élément de réponse associé au tag
                        ResponseItem *ri = ((Response *)response)->getResponseItem(responseTag);

                        if(responseValue != "")
                            ri->setValue(responseValue);

                        if(responseJumpTo.toUpper() == "END") //fin du sondage
                            ri->setResponseItemAction(JUMP_TO, 0);
                        else if(responseJumpTo != "")
                        {
                            bool convertOk;
                            int jumpToId = responseJumpTo.toInt(&convertOk, 10);

                            if(!convertOk)
                                throw new RessourceFormatException("jumpto", BAD_VALUE);

                            ri->setResponseItemAction(JUMP_TO, jumpToId);
                        }

                        responseNode = responseNode.nextSibling();
                    }
                    else
                        throw new RessourceFormatException(nodeTag, UNKNOWN_TAG);
                }

                question->setResponse(response);
            }
            else if(responsesType == "declared")
            {
                ResponseDeclared *response = new ResponseDeclared();

                QString disposition = node.toElement().attribute("disposition");
                if(!disposition.isEmpty())
                {
                    disposition = disposition.toLower();
                    if(disposition == "horizontal")
                        response->setResponsesDisposition(HORIZONTAL);
                    else if(disposition != "vertical")
                        throw new RessourceFormatException("disposition", BAD_VALUE);
                }

                //pour le type declared, il faut créer tous les éléments de réponses
                QDomNode responseNode = node.firstChild();
                while(!responseNode.isNull())
                {
                    QString nodeTag = responseNode.toElement().tagName();
                    if(nodeTag == "response")
                    {
                        //attributs obligatoires : tag, value, label
                        QString responseTag = responseNode.toElement().attribute("tag");
                        if(responseTag == "")
                            throw new RessourceFormatException("tag", MISSING_ATTRIBUTE);

                        QString responseValue = responseNode.toElement().attribute("value");
                        if(responseValue == "")
                            throw new RessourceFormatException("value", MISSING_ATTRIBUTE);

                        ResponseItem *ri = new ResponseItem(responseTag, responseValue);

                        QString responseJumpTo = responseNode.toElement().attribute("jumpto");
                        if(responseJumpTo.toUpper() == "END") //fin du sondage
                            ri->setResponseItemAction(JUMP_TO, 0);
                        else if(responseJumpTo != "")
                        {
                            bool convertOk;
                            int jumpToId = responseJumpTo.toInt(&convertOk, 10);

                            if(!convertOk)
                                throw new RessourceFormatException("jumpto", BAD_VALUE);

                            ri->setResponseItemAction(JUMP_TO, jumpToId);
                        }

                        //seul ces trois tags là peuvent être rédifinis via les noeuds "<response>"
                        bool labelFrTag = false;
                        bool labelNlTag = false;
                        bool labelEnTag = false;

                        QDomNode labelNode = responseNode.firstChild();
                        while(!labelNode.isNull())
                        {
                            QString labelNodeName = labelNode.toElement().tagName();
                            if(labelNodeName == "fr" || labelNodeName == "nl" || labelNodeName == "en") //si noeud <fr>, <nl>, <en>
                            {
                                //on récupère le label
                                QString label = labelNode.toElement().attribute("label").toUtf8();
                                if(label.isEmpty())
                                    throw new RessourceFormatException("label", MISSING_ATTRIBUTE);

                                if(labelNodeName == "fr")
                                {
                                    if(labelFrTag)
                                        throw new RessourceFormatException(responseTag, MULTI_DEF);

                                    ri->addLabel(FR, label);

                                    labelFrTag = true;
                                }
                                else if(labelNodeName == "nl")
                                {
                                    if(labelNlTag)
                                        throw new RessourceFormatException(responseTag, MULTI_DEF);

                                    ri->addLabel(NL, label);

                                    labelNlTag = true;
                                }
                                else if(labelNodeName == "en")
                                {
                                    if(labelEnTag)
                                        throw new RessourceFormatException(responseTag, MULTI_DEF);

                                    ri->addLabel(EN, label);

                                    labelEnTag = true;
                                }
                            }
                            else
                                throw new RessourceFormatException(labelNodeName, UNKNOWN_TAG);
                           
                            labelNode = labelNode.nextSibling();
                        }

                        //s'il manque un des trois noeuds obligatoires
                        if(!labelFrTag || !labelNlTag || !labelEnTag)
                        {
                            QString missingTag;
                            if(!labelFrTag)        missingTag = "fr (label)";
                            else if(!labelNlTag)   missingTag = "nl (label)";
                            else if(!labelEnTag)   missingTag = "en (label)";
                            
                            RessourceFormatException *e = new RessourceFormatException(missingTag, MISSING_TAG);
                            e->setQuestionId(questionId);

                            throw e;
                        }

                        response->addResponseItem(ri);
                        responseNode = responseNode.nextSibling();
                    }
                    else
                        throw new RessourceFormatException(nodeTag, UNKNOWN_TAG);
                }

                question->setResponse(response);
            }
            else if(responsesType == "embedded")
            {
                ResponseEmbedded *response = new ResponseEmbedded();

                //pour le type embedded, il faut créer tous les éléments de réponses
                QDomNode responseNode = node.firstChild();
                while(!responseNode.isNull())
                {
                    QString nodeTag = responseNode.toElement().tagName();
                    if(nodeTag == "response")
                    {
                        //attributs obligatoires : tag, value
                        QString responseTag = responseNode.toElement().attribute("tag");
                        if(responseTag == "")
                            throw new RessourceFormatException("tag", MISSING_ATTRIBUTE);

                        QString responseValue = responseNode.toElement().attribute("value");
                        if(responseValue == "")
                            throw new RessourceFormatException("value", MISSING_ATTRIBUTE);

                        ResponseItem *ri = new ResponseItem(responseTag, responseValue);

                        QString responseJumpTo = responseNode.toElement().attribute("jumpto");
                        if(responseJumpTo.toUpper() == "END") //fin du sondage
                            ri->setResponseItemAction(JUMP_TO, 0);
                        else if(responseJumpTo != "")
                        {
                            bool convertOk;
                            int jumpToId = responseJumpTo.toInt(&convertOk, 10);

                            if(!convertOk)
                                throw new RessourceFormatException("jumpto", BAD_VALUE);

                            ri->setResponseItemAction(JUMP_TO, jumpToId);
                        }

                        response->addResponseItem(ri);
                        responseNode = responseNode.nextSibling();
                    }
                    else
                        throw new RessourceFormatException(nodeTag, UNKNOWN_TAG);
                }

                question->setResponse(response);
            }
            else
                throw new RessourceFormatException(subTagName, UNKNOWN_ATTRIBUTE);

            responsesTag = true;
        }
        else //balise inconnue
            throw new RessourceFormatException(subTagName, UNKNOWN_TAG);

        node = node.nextSibling();
    }

    //s'il manque un des quatre noeuds obligatoires
    if(!frTag || !nlTag || !enTag || !responsesTag)
    {
        QString missingTag;
        if(!frTag)             missingTag = "fr";
        else if(!nlTag)        missingTag = "nl";
        else if(!enTag)        missingTag = "en";
        else if(!responsesTag) missingTag = "responses";

        RessourceFormatException *e = new RessourceFormatException(missingTag, MISSING_TAG);
        e->setQuestionId(questionId);

        throw e;
    }

    m_questions->append(question);
}

void EngineXML::populate(Configuration *config)
{
    config->setQuestions(m_questions);
    config->setNetwork(m_serverAddr, m_serverPort);
    config->setGUI(m_fontFamily, m_borderSize, m_borderColor);
    config->setDBManager(m_pathNormal, m_pathError);
    config->setDevMode(m_devMode);
}

#include "ConfigurationXML.h"


ConfigurationXML::ConfigurationXML(QString path)
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

        m_outputItems = new QVector<OutputItem *>();
        m_error = 0;

        try {
            //on traite le document
            parse(&doc);
        } catch(RessourceException *exception) {
            //un detruit les vecteurs alloués
            qDeleteAll(*m_outputItems);

            delete m_outputItems;
            m_outputItems = NULL;

            throw exception;
        }
    }
    else
        throw new RessourceOpenException(path);
}


ConfigurationXML::~ConfigurationXML() { }

void ConfigurationXML::parse(QDomDocument *doc)
{
    //doit démarrer par le noeud racine "<survey>"
    QDomElement root = doc->documentElement();
    if(root.tagName() != "admissionsurveyserver")
        throw new RessourceParsingException(-1);

    //on parcourt les noeuds "question"
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        QString tagName = node.toElement().tagName();
        if(tagName == "record") //les records
        {
            QString questionIdStr = node.toElement().attribute("questionid");
            QString outputFile = node.toElement().attribute("outputfile");

            if(questionIdStr.isEmpty())
                throw new RessourceFormatException("questionid", MISSING_ATTRIBUTE);

            bool convertOk = false;
            int questionId = questionIdStr.toInt(&convertOk, 10);
            if(!convertOk)
                    throw new RessourceFormatException("questionid", BAD_VALUE);

            if(outputFile.isEmpty())
                throw new RessourceFormatException("outputfile", MISSING_ATTRIBUTE);

            OutputItem *oi = getOutputItem(outputFile);
            oi->addQuestionId(questionId);
        }
        else
            throw new RessourceFormatException(tagName, UNKNOWN_TAG);

        //on passe au noeud suivant
        node = node.nextSibling();
    }


}

QVector<OutputItem *> * ConfigurationXML::getOutputItems()
{
    return m_outputItems;
}

OutputItem * ConfigurationXML::getOutputItem(QString filePath)
{
    for(int i=0; i < m_outputItems->size(); i++)
    {
        if(filePath == m_outputItems->at(i)->getFilePath())
            return m_outputItems->at(i);
    }

    OutputItem *oi = new OutputItem(filePath);
    m_outputItems->append(oi);
    return oi;
}

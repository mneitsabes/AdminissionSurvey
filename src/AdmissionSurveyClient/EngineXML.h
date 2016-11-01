#ifndef ENGINEXML_H
#define ENGINEXML_H

#include <QFile>
#include <QDomNode>
#include <QDomElement>
#include <QDomDocument>
#include <QVector>
#include <QDebug>

#include "EngineXML.h"
#include "Question.h"
#include "RessourceException.h"
#include "RessourceOpenException.h"
#include "RessourceParsingException.h"
#include "RessourceFormatException.h"
#include "RessourceErrorException.h"
#include "ResponseYesNoUnknown.h"
#include "ResponseYesNo.h"
#include "ResponseDeclared.h"
#include "ResponseItem.h"
#include "ResponseEmbedded.h"
#include "Configuration.h"
class Configuration;

/*
 * Partie du moteur permettant la lecture du fichier XML et la création des différents questions et leurs réponses.
 *
 * Voici la description du fichier XML :
 *
 * Le noeud principal du XML est "<survey>" composé de deux sous-noeuds : "<general>" et "<questions>".
 *  Les sous-noeuds de "<general>" sont :
 *    - "<font>" dont seul l'attribut "family" permet de spécifier le nom de la police à utiliser
 *    - "<server>" dont l'attribut "server" donne l'adresse IP du serveur et l'attribut "port", le port de connexion.
 *
 *  Les sous-noeuds composant "<questions>" sont uniquement de type "<question>" dont l'attribut "id" est obligatoire.
 *    Il existe ensuite une balise par langue : fr, nl, en avec un attribut "path" définissant la localisation du fichier dans la langue du noeud.
 *    Un sous-noeud "<responses>" ayant un attribut "type" permet de définir le type de réponses :
 *           - yes_no : réponse "Oui - Non" (<response> facultatif)
 *           - yes_no_unknown : réponse "Oui - Non - Je ne sais pas" ((<response> facultatif)
 *           - declared : les éléments de réponses sont déclarées via les sous-noeuds "<response>"
 *           - embeded : les éléments de réponses sont dans le code HTML et le comportement est spécifié via les sous-noeuds "<response>"
 *       Un sous-noeud "<response>" détermine un élément de réponse pour la question courante. Il est composé des attributs suivants :
 *                    - tag : le tag de la réponse (obligatoire)
 *                    - value : la valeur de la réponse (facultatif pour yes_no et yes_no_unknown)
 *                    - jumto : ID de saut (facultatif) ou END pour terminer le sondage
 *                    - label : le texte de la réponse (facultatif sauf pour declared)
 *
 * Le sous-noeud <response> peut être utilisé pour modifier le comportement par défaut de "yes_no" ou "yes_no_unknown" en ajouter des sauts ou en modificant
 * les valeurs par défauts.
 * Pour un sous-noeud <response> de type "declared", l'attribut "disposition" permet de définir la disposition des réponses, voici les valeurs possibles :
 *    - vertical : réponses disposées verticalement (valeur par défaut si l'attribut n'est pas spécifié)
 *    - horizontal : réponses disposées horizontallement
 */
class EngineXML
{
    private:
        /* Code d'erreur */
        int m_error;

        /* Les questions */
        QVector<Question *> *m_questions;

        /* Mode "en développement" */
        bool m_devMode;

        /* GUI - La police utilisée */
        QString m_fontFamily;

        /* GUI - La taille des bordures */
        int m_borderSize;

        /* GUI - La couleur des bordures */
        QString m_borderColor;

        /* Network - L'adresse du serveur */
        QString m_serverAddr;

        /* Network - Le port du serveur */
        int m_serverPort;

        /* DBManager - Le path normal */
        QString m_pathNormal;

        /* DBManager - Le path d'erreur */
        QString m_pathError;

    public:
        EngineXML(QString path);
        ~EngineXML();
        void parse(QDomDocument *doc);
        void parseGeneral(QDomNode node);
        void parseQuestions(QDomNode questionsNode);
        void parseQuestion(QDomNode questionNode);
        void populate(Configuration *config);
};

#endif // ENGINEXML_H

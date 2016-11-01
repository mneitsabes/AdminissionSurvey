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
 * Partie du moteur permettant la lecture du fichier XML et la cr�ation des diff�rents questions et leurs r�ponses.
 *
 * Voici la description du fichier XML :
 *
 * Le noeud principal du XML est "<survey>" compos� de deux sous-noeuds : "<general>" et "<questions>".
 *  Les sous-noeuds de "<general>" sont :
 *    - "<font>" dont seul l'attribut "family" permet de sp�cifier le nom de la police � utiliser
 *    - "<server>" dont l'attribut "server" donne l'adresse IP du serveur et l'attribut "port", le port de connexion.
 *
 *  Les sous-noeuds composant "<questions>" sont uniquement de type "<question>" dont l'attribut "id" est obligatoire.
 *    Il existe ensuite une balise par langue : fr, nl, en avec un attribut "path" d�finissant la localisation du fichier dans la langue du noeud.
 *    Un sous-noeud "<responses>" ayant un attribut "type" permet de d�finir le type de r�ponses :
 *           - yes_no : r�ponse "Oui - Non" (<response> facultatif)
 *           - yes_no_unknown : r�ponse "Oui - Non - Je ne sais pas" ((<response> facultatif)
 *           - declared : les �l�ments de r�ponses sont d�clar�es via les sous-noeuds "<response>"
 *           - embeded : les �l�ments de r�ponses sont dans le code HTML et le comportement est sp�cifi� via les sous-noeuds "<response>"
 *       Un sous-noeud "<response>" d�termine un �l�ment de r�ponse pour la question courante. Il est compos� des attributs suivants :
 *                    - tag : le tag de la r�ponse (obligatoire)
 *                    - value : la valeur de la r�ponse (facultatif pour yes_no et yes_no_unknown)
 *                    - jumto : ID de saut (facultatif) ou END pour terminer le sondage
 *                    - label : le texte de la r�ponse (facultatif sauf pour declared)
 *
 * Le sous-noeud <response> peut �tre utilis� pour modifier le comportement par d�faut de "yes_no" ou "yes_no_unknown" en ajouter des sauts ou en modificant
 * les valeurs par d�fauts.
 * Pour un sous-noeud <response> de type "declared", l'attribut "disposition" permet de d�finir la disposition des r�ponses, voici les valeurs possibles :
 *    - vertical : r�ponses dispos�es verticalement (valeur par d�faut si l'attribut n'est pas sp�cifi�)
 *    - horizontal : r�ponses dispos�es horizontallement
 */
class EngineXML
{
    private:
        /* Code d'erreur */
        int m_error;

        /* Les questions */
        QVector<Question *> *m_questions;

        /* Mode "en d�veloppement" */
        bool m_devMode;

        /* GUI - La police utilis�e */
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

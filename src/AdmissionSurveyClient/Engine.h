#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>

#include <QVector>
#include <QDebug>
#include <QDir>
#include <QUrl>
#include <QSound>

#include "Question.h"
#include "RessourceException.h"
#include "RessourceFormatException.h"
#include "EngineListener.h"
#include "QuestionData.h"
#include "ResponseYesNoUnknown.h"
#include "ResponseItem.h"
#include "RecorderSession.h"
#include "Record.h"
#include "Language.h"
#include "IllegalArgumentException.h"
#include "NetworkTransfert.h"
#include "NetworkTransfertListener.h"
#include "DBManager.h"
#include "GlobalView.h"
#include "Configuration.h"

extern ostream cout;

/* Localisation du fichier HTML permettant � l'utilisateur de choisir sa langue */
#define ENGINE_CHOOSELANGUAGE_FILE ":/ChooseLanguage.html"

/* Localisation du fichier HTML affichant le message de fin en n�erlandais */
#define ENGINE_ENDMESSAGE_NL_FILE ":/EndMessage_NL.html"

/* Localisation du fichier HTML affichant le message de fin en fran�ais */
#define ENGINE_ENDMESSAGE_FR_FILE ":/EndMessage_FR.html"

/* Localisation du fichier HTML affichant le message de fin en anglais */
#define ENGINE_ENDMESSAGE_EN_FILE ":/EndMessage_EN.html"

/* Localisation du fichier HTML affichant le message demandant le num�ro d'admission du patient */
#define ENGINE_ASKNUMADM_FILE ":/AskNumAdm.html"

/*
 * Les diff�rentes �tapes du processus de sondage :
 *
 *  - demande la langue de l'utilisateur
 *  - les diff�rentes questions
 *  - le message de fin
 *  - encodage du num�ro d'admission
 */
enum EngineStep {
   /* Choix de la langue par l'utilisateur */
   CHOOSE_LANGUAGE,

   /* Phase o� les diff�rentes questions sont pos�es */
   SURVEY,

   /* Message de fin */
   END,

   /* Encodage du num�ro d'admission */
   ENTER_NUM_ADM,

   /* Sauvegarde locale et distance des r�sultats */
   SAVING_RECORDS
};

/*
 * Moteur de traitement du sondage d'admission.
 *
 * Le moteur initialise sa configuration � partir d'un fichier XML d�crivant les diff�rentes questions et les r�ponses associ�es.
 * Les questions sont repr�sent�es au format HTML. Certains fichiers sont des ressources int�gr�es � l'ex�cutable.
 * La vue transmet les donn�es de l'utilisateur � travers des URLs cliqu�es ou des valeurs pures.
 *
 * Une session comprend les �tapes suivantes :
 *   - choix de la langue par l'utilisateur (� partir d'un fichier unique)
 *   - pose les questions � l'utilisateur
 *        . enregistre les r�ponses
 *        . en fonction des r�ponses, passe � la question suivante ou saute directement � une question pr�cise
 *   - affiche un message de fin de sondage
 *        . lorsque le resposnable presse la touche "ENTER", le syst�me passe � l'encodage du num�ro d'admssion
 *   - encodage du num�ro d'admission
 *   - sauvegarde locale et distance des r�sultats + nouvelle session
 *
 * Le moteur �coute le module r�seau pour conna�tre l'�tat de la sauvegarde distante. Une fois le r�sultat obtenu, une nouvelle session est lanc�e.
 */
class Engine : public NetworkTransfertListener
{
    private:
        /* La configuration */
        Configuration *m_config;

        /* Les �couteurs */
        QVector<EngineListener *> *m_listeners;

        /* L'�tape actuelle */
        EngineStep m_engineStep;

        /* La question actuelle */
        Question *m_currentQuestion;

        /* L'enregistreur des r�ponses pour la session courante */
        RecorderSession *m_recorderSession;

        /* La langue choisie pour la session courante */
        Language m_currentLg;

        /* Module de transfert des donn�es au serveur */
        NetworkTransfert *m_neworkTransfert;

        /* Gestionnaire de sauvegardes locales */
        DBManager *m_dbManager;

        /* Le sound jou� lorsque le patient est d�tect� comme � risque */
        //QSound *m_sound;

    public:
        Engine(QString pathXML);
        virtual ~Engine();
        void addEngineListener(EngineListener *listener);
        void removeEngineListener(EngineListener *listener);
        QuestionData getCurrentQuestionData();
        void elementClicked(QUrl url);
        void transfertUserData(QString value);
        void keyPressed(int key);
        void recordTransfertSuccessful(QString uniqueId);
        void recordTransfertError(QString uniqueId, QString errorMsg);
        int getPercentProgress();
        Configuration * getConfiguration();

    private:
        void notifyListeners();
        void setCurrentQuestion(Question *question);
        void newSession();
        void saveRemote();
        Question * getCurrentQuestion();
        QByteArray getDataTopPage();
        QByteArray getDataBottomPage();
};

#endif // ENGINE_H

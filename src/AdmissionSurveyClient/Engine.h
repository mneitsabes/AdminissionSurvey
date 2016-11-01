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

/* Localisation du fichier HTML permettant à l'utilisateur de choisir sa langue */
#define ENGINE_CHOOSELANGUAGE_FILE ":/ChooseLanguage.html"

/* Localisation du fichier HTML affichant le message de fin en néerlandais */
#define ENGINE_ENDMESSAGE_NL_FILE ":/EndMessage_NL.html"

/* Localisation du fichier HTML affichant le message de fin en français */
#define ENGINE_ENDMESSAGE_FR_FILE ":/EndMessage_FR.html"

/* Localisation du fichier HTML affichant le message de fin en anglais */
#define ENGINE_ENDMESSAGE_EN_FILE ":/EndMessage_EN.html"

/* Localisation du fichier HTML affichant le message demandant le numéro d'admission du patient */
#define ENGINE_ASKNUMADM_FILE ":/AskNumAdm.html"

/*
 * Les différentes étapes du processus de sondage :
 *
 *  - demande la langue de l'utilisateur
 *  - les différentes questions
 *  - le message de fin
 *  - encodage du numéro d'admission
 */
enum EngineStep {
   /* Choix de la langue par l'utilisateur */
   CHOOSE_LANGUAGE,

   /* Phase où les différentes questions sont posées */
   SURVEY,

   /* Message de fin */
   END,

   /* Encodage du numéro d'admission */
   ENTER_NUM_ADM,

   /* Sauvegarde locale et distance des résultats */
   SAVING_RECORDS
};

/*
 * Moteur de traitement du sondage d'admission.
 *
 * Le moteur initialise sa configuration à partir d'un fichier XML décrivant les différentes questions et les réponses associées.
 * Les questions sont représentées au format HTML. Certains fichiers sont des ressources intégrées à l'exécutable.
 * La vue transmet les données de l'utilisateur à travers des URLs cliquées ou des valeurs pures.
 *
 * Une session comprend les étapes suivantes :
 *   - choix de la langue par l'utilisateur (à partir d'un fichier unique)
 *   - pose les questions à l'utilisateur
 *        . enregistre les réponses
 *        . en fonction des réponses, passe à la question suivante ou saute directement à une question précise
 *   - affiche un message de fin de sondage
 *        . lorsque le resposnable presse la touche "ENTER", le système passe à l'encodage du numéro d'admssion
 *   - encodage du numéro d'admission
 *   - sauvegarde locale et distance des résultats + nouvelle session
 *
 * Le moteur écoute le module réseau pour connaître l'état de la sauvegarde distante. Une fois le résultat obtenu, une nouvelle session est lancée.
 */
class Engine : public NetworkTransfertListener
{
    private:
        /* La configuration */
        Configuration *m_config;

        /* Les écouteurs */
        QVector<EngineListener *> *m_listeners;

        /* L'étape actuelle */
        EngineStep m_engineStep;

        /* La question actuelle */
        Question *m_currentQuestion;

        /* L'enregistreur des réponses pour la session courante */
        RecorderSession *m_recorderSession;

        /* La langue choisie pour la session courante */
        Language m_currentLg;

        /* Module de transfert des données au serveur */
        NetworkTransfert *m_neworkTransfert;

        /* Gestionnaire de sauvegardes locales */
        DBManager *m_dbManager;

        /* Le sound joué lorsque le patient est détecté comme à risque */
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

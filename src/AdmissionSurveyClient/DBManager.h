#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

#include "RecorderSession.h"
#include "Configuration.h"

/*
 * Le type d'�criture.
 */
enum WriteType
{
    /* La sauvegarde distante s'est bien pass�e : on sauve dans DBMANAGER_PATH_NORMAL */
    NORMAL,

    /* La sauvegarde distante a provoqu� une erreur : on sauve dans DBMANAGER_PATH_ERROR */
    ERROR
};

/*
 * Le gestionnaire de sauvegarde des r�sultats dans des fichiers locaux.
 *
 * Permet de sauver les r�sultats d'une session dans des fichiers locaux au format CSV. Voici la structure d'une ligne :
 *    <ID unique de session>;<date format dd/mm/yyy>;<heure format hh:mm>;<valeur Q1>;<valeur Q2>;<valeur Qn>;<num�ro d'admission>
 * Si la sauvegarde distante a provoqu� une erreur, l'enregistrement se fait dans un fichier d'erreur, sinon dans un fichier de sauvegarde normal.
 */
class DBManager
{
    private:
        Configuration *m_config;

    public:
        DBManager(Configuration *config);
        void save(WriteType writeType, RecorderSession *recorderSession);
};

#endif // DBMANAGER_H

#include <iostream>

#include <QtCore>
#include <QApplication>
#include <QDebug>

#include "Engine.h"
#include "GlobalView.h"

extern ostream cout;

/*
 * Point d'entr�e d'AdmissionSurvey.
 *
 * Initialise le moteur et ensuite la vue utilis�e.
 * Il faut passer un argument au programme :
 *   - 1er : le chemin d'acc�s au fichier XML d�crivant les questions
 *
 * @argc nombre d'arguments
 * @argv arguments
 * @return code de retour de fin d'ex�cution
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    try {
        //r�cup�re le premier argument passe au programme comme �tant le chemin du fichier XML utilis� pour les questions
        QString pathXML = "";
        if(app.arguments().size() > 1)
            pathXML = app.arguments().at(1);

        if(!pathXML.isEmpty()) //dans ce cas, on continue
        {
            //moteur
            Engine *engine = new Engine(pathXML);

            //vue utilisateur
            GlobalView *globalView = new GlobalView(engine);

            if(!engine->getConfiguration()->isDevMode())
                globalView->showFullScreen();
            else
                globalView->show();
        }
        else //dans ce cas, pas la peine d'aller plus loin
        {
            std::cout << "AdmissionSurvey <question's xml file> <server addr>" << std::endl;
            return -1;
        }
    } catch(exception *e) {
            std::cerr << "[Exception] " << e->what() << std::endl;
            return -1;
    }

    return app.exec();
}

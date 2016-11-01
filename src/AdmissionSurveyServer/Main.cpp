#include <iostream>

#include <QCoreApplication>
#include <QDebug>
#include <QStringList>

#include "AdmissionSurveyServer.h"
#include "IllegalArgumentException.h"

extern ostream cout;

/*
 * Point d'entr�e d'AdmissionSurveyServer.
 *
 * Initialise le serveur et lance l'�coute sur le port ad�quat.
 * Il faut passer deux arguments au programme :
 *   - 1er : le chemin d'acc�s au o� seront stock�es les r�sultats envoy�s par les clients
 *
 * @argc nombre d'arguments
 * @argv arguments
 * @return code de retour de fin d'ex�cution
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    try {
        QString pathResults = "";
        QString pathConfig = "";
        if(app.arguments().size() > 2)
        {
            pathResults = app.arguments().at(1);
            pathConfig = app.arguments().at(2);
        }

        if(!pathResults.isEmpty() && !pathConfig.isEmpty()) //dans ce cas, on continue
        {
            //initialisation du serveur
            Configuration *configuration = new Configuration(pathConfig);
            AdmissionSurveyServer *server = new AdmissionSurveyServer(configuration, pathResults);

            //lance l'�coute sur le port ad�quat et sur toutes les adresses r�seau du syst�me
            if(!server->listen(QHostAddress::Any, ADMISSIONSURVEYSERVER_PORT))
            {
                std::cerr << "Failed to bind the port ADMISSIONSURVEYSERVER_PORT " << std::endl;
                return -1;
            }
        }
        else
        {
            std::cout << "AdmissionSurveyServer <results's file>" << std::endl;
            return -1;
        }
    } catch(exception *e) {
            std::cerr << "[Exception] " << e->what() << std::endl;
            return -1;
    }

    return app.exec();
}

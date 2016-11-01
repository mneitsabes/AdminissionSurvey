#include <iostream>

#include <QCoreApplication>
#include <QDebug>
#include <QStringList>

#include "AdmissionSurveyServer.h"
#include "IllegalArgumentException.h"

extern ostream cout;

/*
 * Point d'entrée d'AdmissionSurveyServer.
 *
 * Initialise le serveur et lance l'écoute sur le port adéquat.
 * Il faut passer deux arguments au programme :
 *   - 1er : le chemin d'accès au où seront stockées les résultats envoyés par les clients
 *
 * @argc nombre d'arguments
 * @argv arguments
 * @return code de retour de fin d'exécution
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

            //lance l'écoute sur le port adéquat et sur toutes les adresses réseau du système
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

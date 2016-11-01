#include "Tools.h"

/*
 * Lit le contenu du fichier.
 *
 * @param path la localisation du fichier
 * @return les données du fichier
 */
QByteArray Tools::readAllFile(QString path)
{
    QFile file(path);
    if(file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        file.close();

        return data;
    }
    else
        throw new IllegalArgumentException("File doesn't exist");
}

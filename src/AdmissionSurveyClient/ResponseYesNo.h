#ifndef RESPONSEYESNO_H
#define RESPONSEYESNO_H

#include <QDebug>

#include "Response.h"
#include "ResponseItem.h"
#include "Tools.h"
#include "Language.h"

/* Localisation du fichier des donn�es HTML en fran�ais */
#define RESPONSEYESNO_FR_FILE ":/ResponseYesNo_FR.html"

/* Localisation du fichier des donn�es HTML en n�erlandais */
#define RESPONSEYESNO_NL_FILE ":/ResponseYesNo_NL.html"

/* Localisation du fichier des donn�es HTML en anglais */
#define RESPONSEYESNO_EN_FILE ":/ResponseYesNo_EN.html"

/* Tag de r�ponse "oui" */
#define RESPONSEYESNO_TAG_YES "RESPONSE:YES"

/* Tag de r�ponse "non" */
#define RESPONSEYESNO_TAG_NO "RESPONSE:NO"

/*
 * R�ponse pr�d�finie de type "Oui - Non".
 *
 * L'�l�ment de r�ponse "Oui" a la valeur "1".
 * L'�l�ment de r�ponse "Non" a la valeur "0".
 */
class ResponseYesNo : public Response
{
    public:
        ResponseYesNo();
        ~ResponseYesNo();
        QByteArray getHTMLData(Language lg);
};

#endif // RESPONSEYESNO_H

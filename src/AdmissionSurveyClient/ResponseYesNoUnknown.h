#ifndef RESPONSEYESNOUNKNOWN_H
#define RESPONSEYESNOUNKNOWN_H

#include "Response.h"
#include "ResponseItem.h"
#include "Tools.h"
#include "Language.h"

/* Localisation du fichier des donn�es HTML en fran�ais */
#define RESPONSEYESNOUNKNOWN_FR_FILE ":/ResponseYesNoUnknown_FR.html"

/* Localisation du fichier des donn�es HTML en n�erlandais */
#define RESPONSEYESNOUNKNOWN_NL_FILE ":/ResponseYesNoUnknown_NL.html"

/* Localisation du fichier des donn�es HTML en anglais */
#define RESPONSEYESNOUNKNOWN_EN_FILE ":/ResponseYesNoUnknown_EN.html"

/* Tag de r�ponse "oui" */
#define RESPONSEYESNOUNKNOWN_TAG_YES "RESPONSE:YES"

/* Tag de r�ponse "non" */
#define RESPONSEYESNOUNKNOWN_TAG_NO "RESPONSE:NO"

/* Tag de r�ponse "je ne sais pas" */
#define RESPONSEYESNOUNKNOWN_TAG_UNKNOWN "RESPONSE:UNKNOWN"

/*
 * R�ponse pr�d�finie de type "Oui - Non - Je ne sais pas".
 *
 * L'�l�ment de r�ponse "Oui" a la valeur "1".
 * L'�l�ment de r�ponse "Non" a la valeur "0".
 * L'�l�ment de r�ponse "Je ne sais pas" a la valeur "2".
 */
class ResponseYesNoUnknown : public Response
{
    public:
        ResponseYesNoUnknown();
        ~ResponseYesNoUnknown();
        QByteArray getHTMLData(Language lg);
};

#endif // RESPONSEYESNOUNKNOWN_H

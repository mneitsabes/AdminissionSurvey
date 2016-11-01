#ifndef RESPONSEYESNOUNKNOWN_H
#define RESPONSEYESNOUNKNOWN_H

#include "Response.h"
#include "ResponseItem.h"
#include "Tools.h"
#include "Language.h"

/* Localisation du fichier des données HTML en français */
#define RESPONSEYESNOUNKNOWN_FR_FILE ":/ResponseYesNoUnknown_FR.html"

/* Localisation du fichier des données HTML en néerlandais */
#define RESPONSEYESNOUNKNOWN_NL_FILE ":/ResponseYesNoUnknown_NL.html"

/* Localisation du fichier des données HTML en anglais */
#define RESPONSEYESNOUNKNOWN_EN_FILE ":/ResponseYesNoUnknown_EN.html"

/* Tag de réponse "oui" */
#define RESPONSEYESNOUNKNOWN_TAG_YES "RESPONSE:YES"

/* Tag de réponse "non" */
#define RESPONSEYESNOUNKNOWN_TAG_NO "RESPONSE:NO"

/* Tag de réponse "je ne sais pas" */
#define RESPONSEYESNOUNKNOWN_TAG_UNKNOWN "RESPONSE:UNKNOWN"

/*
 * Réponse prédéfinie de type "Oui - Non - Je ne sais pas".
 *
 * L'élément de réponse "Oui" a la valeur "1".
 * L'élément de réponse "Non" a la valeur "0".
 * L'élément de réponse "Je ne sais pas" a la valeur "2".
 */
class ResponseYesNoUnknown : public Response
{
    public:
        ResponseYesNoUnknown();
        ~ResponseYesNoUnknown();
        QByteArray getHTMLData(Language lg);
};

#endif // RESPONSEYESNOUNKNOWN_H

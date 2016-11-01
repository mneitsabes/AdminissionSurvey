#ifndef RESPONSEYESNO_H
#define RESPONSEYESNO_H

#include <QDebug>

#include "Response.h"
#include "ResponseItem.h"
#include "Tools.h"
#include "Language.h"

/* Localisation du fichier des données HTML en français */
#define RESPONSEYESNO_FR_FILE ":/ResponseYesNo_FR.html"

/* Localisation du fichier des données HTML en néerlandais */
#define RESPONSEYESNO_NL_FILE ":/ResponseYesNo_NL.html"

/* Localisation du fichier des données HTML en anglais */
#define RESPONSEYESNO_EN_FILE ":/ResponseYesNo_EN.html"

/* Tag de réponse "oui" */
#define RESPONSEYESNO_TAG_YES "RESPONSE:YES"

/* Tag de réponse "non" */
#define RESPONSEYESNO_TAG_NO "RESPONSE:NO"

/*
 * Réponse prédéfinie de type "Oui - Non".
 *
 * L'élément de réponse "Oui" a la valeur "1".
 * L'élément de réponse "Non" a la valeur "0".
 */
class ResponseYesNo : public Response
{
    public:
        ResponseYesNo();
        ~ResponseYesNo();
        QByteArray getHTMLData(Language lg);
};

#endif // RESPONSEYESNO_H

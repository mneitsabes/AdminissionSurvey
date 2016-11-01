#include "ResponseYesNoUnknown.h"

/*
 * Constructeur.
 */
ResponseYesNoUnknown::ResponseYesNoUnknown() : Response(YES_NO_UNKNOWN)
{
    ResponseItem *riYes = new ResponseItem(RESPONSEYESNOUNKNOWN_TAG_YES, "1");
    ResponseItem *riNo = new ResponseItem(RESPONSEYESNOUNKNOWN_TAG_NO, "0");
    ResponseItem *riUnknown = new ResponseItem(RESPONSEYESNOUNKNOWN_TAG_UNKNOWN, "2");

    m_responseItems.append(riYes);
    m_responseItems.append(riNo);
    m_responseItems.append(riUnknown);
}

/*
 * Destructeur.
 */
ResponseYesNoUnknown::~ResponseYesNoUnknown() { }

/*
 * Retourne les donn�es HTML de la r�ponse en fonction de la langue sp�cifi�e.
 *
 * @param lg la langue
 * @return donn�es HTML
 */
QByteArray ResponseYesNoUnknown::getHTMLData(Language lg)
{
    QByteArray data;

    if(lg == NL)
        data = Tools::readAllFile(RESPONSEYESNOUNKNOWN_NL_FILE);
    else if(lg == EN)
        data = Tools::readAllFile(RESPONSEYESNOUNKNOWN_EN_FILE);
    else
        data = Tools::readAllFile(RESPONSEYESNOUNKNOWN_FR_FILE);

    return data;
}


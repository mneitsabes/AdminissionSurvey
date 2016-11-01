#include "ResponseYesNo.h"

/*
 * Constructeur.
 */
ResponseYesNo::ResponseYesNo() : Response(YES_NO)
{
    ResponseItem *riYes = new ResponseItem(RESPONSEYESNO_TAG_YES, "1");
    ResponseItem *riNo = new ResponseItem(RESPONSEYESNO_TAG_NO, "0");

    m_responseItems.append(riYes);
    m_responseItems.append(riNo);
}

/*
 * Destructeur.
 */
ResponseYesNo::~ResponseYesNo() { }

/*
 * Retourne les donn�es HTML de la r�ponse en fonction de la langue sp�cifi�e.
 *
 * @param lg la langue
 * @return donn�es HTML
 */
QByteArray ResponseYesNo::getHTMLData(Language lg)
{
    QByteArray data;

    if(lg == NL)
        data = Tools::readAllFile(RESPONSEYESNO_NL_FILE);
    else if(lg == EN)
        data = Tools::readAllFile(RESPONSEYESNO_EN_FILE);
    else
        data = Tools::readAllFile(RESPONSEYESNO_FR_FILE);

    return data;
}

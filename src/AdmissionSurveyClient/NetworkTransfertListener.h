#ifndef NETWORKTRANSFERTLISTENER_H
#define NETWORKTRANSFERTLISTENER_H

/*
 * Ecouteur du module de transfert réseau.
 *
 * Lorsque le module de transfert a terminé, la méthode recordTransfertSuccessful() ou recordTransfertError().
 */
class NetworkTransfertListener
{
    public:
        /*
         * Si le transfert est réussi.
         *
         * @param uniqueId l'ID unique de l'enregistrement
         */
        virtual void recordTransfertSuccessful(QString uniqueId) = 0;

        /*
         * Si le transfert a échoué avec le message d'erreur.
         *
         * @param uniqueId l'ID unique de l'enregistrement
         * @param errorMsg le message d'erreur.
         */
        virtual void recordTransfertError(QString uniqueId, QString errorMsg) = 0;
};


#endif // NETWORKTRANSFERTLISTENER_H

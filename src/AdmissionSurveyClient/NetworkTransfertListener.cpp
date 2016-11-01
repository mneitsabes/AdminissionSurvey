#include "NetworkTransfertListener.h"

/*
 * Ecouteur du moteur.
 *
 * Lorsque le moteur change d'�tat, la m�thode engineStateChanged() est appel�e.
 */
class EngineListener
{
    public:
        /*
         * Indique que le moteur a chang� d'�tat.
         */
        virtual void engineStateChanged() = 0;
};

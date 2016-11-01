#ifndef ENGINELISTENER_H
#define ENGINELISTENER_H

/*
 * Ecouteur du moteur.
 *
 * Lorsque le moteur change d'état, la méthode engineStateChanged() est appelée.
 */
class EngineListener
{
    public:
        /*
         * Indique que le moteur a changé d'état.
         */
        virtual void engineStateChanged() = 0;
};

#endif // ENGINELISTENER_H

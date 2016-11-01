#ifndef QWEBVIEWSELECTIONSUPPRESSOR_H
#define QWEBVIEWSELECTIONSUPPRESSOR_H

#include <QObject>
#include <QWebView>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>

/*
 * Permet de supprimer la s�lection du texte de la vue web sur laquelle l'objet s'applique.
 */
class QWebViewSelectionSuppressor : public QObject
{
    Q_OBJECT

    private:
        /* La vue web impact�e */
        QWebView *m_webView;

        /* Indique si le bouton de la souris est press� */
        bool m_mousePressed;

    public:
        QWebViewSelectionSuppressor(QWebView *webView);

    protected:
        bool eventFilter(QObject *object, QEvent *event);

 };

#endif // QWEBVIEWSELECTIONSUPPRESSOR_H

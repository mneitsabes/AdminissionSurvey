#include "QWebViewSelectionSuppressor.h"

/*
 * Constructeur.
 *
 * @param webView la vue web impact�e
 */
QWebViewSelectionSuppressor::QWebViewSelectionSuppressor(QWebView *webView) : QObject(webView)
{
    m_webView = webView;
    m_mousePressed = false;

    m_webView->installEventFilter(this);
}

/*
 * Gestion des �v�nements.
 *
 * @param object l'objet
 * @param event l'�v�nement
 * @return vrai ou faux
 */
bool QWebViewSelectionSuppressor::eventFilter(QObject *object, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::MouseButtonPress:
            if(static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton)
                m_mousePressed = true;
        break;
        case QEvent::MouseButtonRelease:
            if(static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton)
                m_mousePressed = false;
        break;
        case QEvent::MouseMove:
            if(m_mousePressed)
                return true;
        break;
        default:
        break;
    }

    return false;
}

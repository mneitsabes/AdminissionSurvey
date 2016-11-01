#ifndef GLOBALVIEW_H
#define GLOBALVIEW_H

#include <QWidget>
#include <QtGui>
#include <QRect>
#include <QPen>
#include <QBrush>
#include <QWebView>
#include <QNetworkRequest>
#include <QWebFrame>
#include <QPixmap>
#include <QCursor>

#include "Engine.h"
class Engine;
#include "Question.h"
#include "QWebViewSelectionSuppressor.h"
#include "Configuration.h"

/*
 * Fen�tre principale de l'application servant � afficher les vues � l'utilisateur.
 *
 * Cette fen�tre principale encapsule une vue du moteur de rendu Webkit permettant d'afficher des vues � base de code HTML.
 * La vue �coute le moteur et d�s que l'�tat change, il actualise le contenu � afficher � l'utilisateur.
 * Elle relaye les �v�nements suivants au moteur :
 *   - clic d'un utilisateur sur un lien
 *   - touche press�e
 * La m�thode transfertString(QString) est invocable � partir d'un script JavaScript pr�sent dans une des vues.
 */
class GlobalView : public QWidget, public EngineListener
{
    Q_OBJECT

    private:
        /* La vue de rendu Webkit */
        QWebView *m_webView;

        /* Le moteur */
        Engine *m_engine;

        /* La configuration */
        Configuration *m_config;

    public:
        GlobalView(Engine *engine, QWidget *parent = 0);
        ~GlobalView();
        void engineStateChanged();
        Q_INVOKABLE void transfertString(QString string);
        //void test123();

    private:
        QRect getEncapsuledFrameGeometry();

    protected:
        void paintEvent(QPaintEvent *event);
        void resizeEvent(QResizeEvent *event);
        void keyReleaseEvent(QKeyEvent *event);

    public slots:
        void linkClicked(const QUrl &url);    
};

#endif // GLOBALVIEW_H

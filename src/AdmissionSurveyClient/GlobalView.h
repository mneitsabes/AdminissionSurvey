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
 * Fenêtre principale de l'application servant à afficher les vues à l'utilisateur.
 *
 * Cette fenêtre principale encapsule une vue du moteur de rendu Webkit permettant d'afficher des vues à base de code HTML.
 * La vue écoute le moteur et dès que l'état change, il actualise le contenu à afficher à l'utilisateur.
 * Elle relaye les évènements suivants au moteur :
 *   - clic d'un utilisateur sur un lien
 *   - touche pressée
 * La méthode transfertString(QString) est invocable à partir d'un script JavaScript présent dans une des vues.
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

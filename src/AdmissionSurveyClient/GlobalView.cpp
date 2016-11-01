#include "GlobalView.h"

/*
 * Construit la vue à partir du moteur.
 *
 * @param engine le moteur
 * @parent le QWidget parent, normalement à NULL
 */
GlobalView::GlobalView(Engine *engine, QWidget *parent) : QWidget(parent)
{
    m_webView = new QWebView(this);
    m_webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks); //émettre le signal linkClicked(QUrl) lorsque l'utilisateur clique sur un lien
    new QWebViewSelectionSuppressor(m_webView); //désactive la sélection de texte

    connect(m_webView->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));

    m_engine = engine;
    m_engine->addEngineListener(this);

    m_config = m_engine->getConfiguration();

    engineStateChanged();

    setGeometry(15, 15, 1280, 1024); //taille de base lorsque l'on ne travaille pas en plein écran (développement)

    //on supprime le pointeur de la souris si l'on n'est pas en développement
    if(!m_config->isDevMode())
    {
        QCursor cursor(Qt::BlankCursor);

        QWidget::setCursor(cursor);
        m_webView->setCursor(cursor);

        #ifdef Q_OS_LINUX
            m_webView->grabMouse(cursor);
        #endif
    }
}

/*
 * Destructeur.
 */
GlobalView::~GlobalView()
{
    delete m_webView;
    m_webView = NULL;

    delete m_engine;
    m_engine = NULL;
}

/*
 * Retourne la zone de la vue Webkit encapsulée.
 *
 * Il s'agit de la taille de la fenêtre moins les bordures.
 *
 * @return la zone
 */
QRect GlobalView::getEncapsuledFrameGeometry()
{
    int borderSize = m_config->getGUI().borderSize;

    return QRect(borderSize,
                 borderSize,
                 width() - (2 * borderSize),
                 height() - (2 * borderSize));
}

/*
 * Indique que le moteur a changé d'état.
 *
 * La vue est actualisée pour refléter l'état du moteur.
 */
void GlobalView::engineStateChanged()
{
    //récupération des données pour la vue Webkit
    QuestionData questionData = m_engine->getCurrentQuestionData();
    m_webView->setHtml(QString::fromUtf8(questionData.getData()), questionData.getBaseUrl());

    //injection du code JavaScript permettant d'invoquer la méthode transfertString(QString) depuis la vue HTML
    m_webView->page()->mainFrame()->addToJavaScriptWindowObject("globalView", this);

    update();
}

/*
 * Evènement de dessin de la fenêtre.
 *
 * Utilisée pour customiser la fenêtre en dessinant les bordures.
 *
 * On aurait pu peindre l'entierté du fond et simplement placer le QWebView à sa bonne position (ce qui aurait donné l'effet des bordures).
 * Cette méthode, plus longue, permet une évolution éventuelle future.
 *
 * @param event l'évènement générant le dessin de la fenêtre
 */
void GlobalView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //dessine le fond de la fenêtre en blanc
    //QColor colorBg("#FFFFFF");
    QColor colorBg(m_config->getGUI().borderColor);

    QPen pen = QPen(colorBg);
    QBrush brush = QBrush(colorBg);

    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, width(), height());

    //dessine les différentes bordures
    QColor colorBorder = m_config->getGUI().borderColor;

    pen.setColor(colorBorder);
    brush.setColor(colorBorder);

    painter.setPen(pen);
    painter.setBrush(brush);

    int borderSize = m_config->getGUI().borderSize;
    QRect topBorder = QRect(0, 0, width()-1, borderSize);
    QRect bottomBorder = QRect(0, height()-borderSize-1, width()-1, borderSize);
    QRect leftBorder = QRect(0, 0, borderSize, height()-1);
    QRect rightBorder = QRect(width()-1-borderSize, 0, borderSize, height()-1);

    painter.drawRect(topBorder);
    painter.drawRect(bottomBorder);
    painter.drawRect(leftBorder);
    painter.drawRect(rightBorder);

    //passe la main à la méthode d'origine pour l'inclusion de la vue Webkit
    QWidget::paintEvent(event);
}

/*
 * Evènement de redimenssionnement de la fenêtre.
 *
 * Utile que lorsque la fenêtre n'est pas lancée en plein écran (phase de développement).
 *
 * @param event l'évènement générant le redimenssionnement
 */
void GlobalView::resizeEvent(QResizeEvent *event)
{
    m_webView->setGeometry(getEncapsuledFrameGeometry());
    update();
}

/*
 * Méthode appelée lorsque l'utilisateur clique sur un lien HTML.
 *
 * Le moteur est prévenu de l'action.
 *
 * @param url l'URL du lien cliqué
 */
void GlobalView::linkClicked(const QUrl &url)
{
    m_engine->elementClicked(url);
}

/*
 * Methode appelée lorsque l'utilisateur relache une touche du clavier.
 *
 * Le moteur est prévenu de l'action.
 *
 * @param event l'évènement lié à l'appui de la touche
 */
void GlobalView::keyReleaseEvent(QKeyEvent *event)
{
    m_engine->keyPressed(event->key()); //le keycode seulement est envoyé au moteur
}

/*
 * Méthode invocable par du code JavaScript au sein de la vue Webkit.
 *
 * La chaine fournie est directement transférée au moteur.
 */
void GlobalView::transfertString(QString string)
{
    m_engine->transfertUserData(string);
}

/*void GlobalView::test123()
{
    m_webView->page()->mainFrame()->evaluateJavaScript("test123();");
}*/

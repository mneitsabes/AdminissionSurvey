#include "GlobalView.h"

/*
 * Construit la vue � partir du moteur.
 *
 * @param engine le moteur
 * @parent le QWidget parent, normalement � NULL
 */
GlobalView::GlobalView(Engine *engine, QWidget *parent) : QWidget(parent)
{
    m_webView = new QWebView(this);
    m_webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks); //�mettre le signal linkClicked(QUrl) lorsque l'utilisateur clique sur un lien
    new QWebViewSelectionSuppressor(m_webView); //d�sactive la s�lection de texte

    connect(m_webView->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));

    m_engine = engine;
    m_engine->addEngineListener(this);

    m_config = m_engine->getConfiguration();

    engineStateChanged();

    setGeometry(15, 15, 1280, 1024); //taille de base lorsque l'on ne travaille pas en plein �cran (d�veloppement)

    //on supprime le pointeur de la souris si l'on n'est pas en d�veloppement
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
 * Retourne la zone de la vue Webkit encapsul�e.
 *
 * Il s'agit de la taille de la fen�tre moins les bordures.
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
 * Indique que le moteur a chang� d'�tat.
 *
 * La vue est actualis�e pour refl�ter l'�tat du moteur.
 */
void GlobalView::engineStateChanged()
{
    //r�cup�ration des donn�es pour la vue Webkit
    QuestionData questionData = m_engine->getCurrentQuestionData();
    m_webView->setHtml(QString::fromUtf8(questionData.getData()), questionData.getBaseUrl());

    //injection du code JavaScript permettant d'invoquer la m�thode transfertString(QString) depuis la vue HTML
    m_webView->page()->mainFrame()->addToJavaScriptWindowObject("globalView", this);

    update();
}

/*
 * Ev�nement de dessin de la fen�tre.
 *
 * Utilis�e pour customiser la fen�tre en dessinant les bordures.
 *
 * On aurait pu peindre l'entiert� du fond et simplement placer le QWebView � sa bonne position (ce qui aurait donn� l'effet des bordures).
 * Cette m�thode, plus longue, permet une �volution �ventuelle future.
 *
 * @param event l'�v�nement g�n�rant le dessin de la fen�tre
 */
void GlobalView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //dessine le fond de la fen�tre en blanc
    //QColor colorBg("#FFFFFF");
    QColor colorBg(m_config->getGUI().borderColor);

    QPen pen = QPen(colorBg);
    QBrush brush = QBrush(colorBg);

    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, width(), height());

    //dessine les diff�rentes bordures
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

    //passe la main � la m�thode d'origine pour l'inclusion de la vue Webkit
    QWidget::paintEvent(event);
}

/*
 * Ev�nement de redimenssionnement de la fen�tre.
 *
 * Utile que lorsque la fen�tre n'est pas lanc�e en plein �cran (phase de d�veloppement).
 *
 * @param event l'�v�nement g�n�rant le redimenssionnement
 */
void GlobalView::resizeEvent(QResizeEvent *event)
{
    m_webView->setGeometry(getEncapsuledFrameGeometry());
    update();
}

/*
 * M�thode appel�e lorsque l'utilisateur clique sur un lien HTML.
 *
 * Le moteur est pr�venu de l'action.
 *
 * @param url l'URL du lien cliqu�
 */
void GlobalView::linkClicked(const QUrl &url)
{
    m_engine->elementClicked(url);
}

/*
 * Methode appel�e lorsque l'utilisateur relache une touche du clavier.
 *
 * Le moteur est pr�venu de l'action.
 *
 * @param event l'�v�nement li� � l'appui de la touche
 */
void GlobalView::keyReleaseEvent(QKeyEvent *event)
{
    m_engine->keyPressed(event->key()); //le keycode seulement est envoy� au moteur
}

/*
 * M�thode invocable par du code JavaScript au sein de la vue Webkit.
 *
 * La chaine fournie est directement transf�r�e au moteur.
 */
void GlobalView::transfertString(QString string)
{
    m_engine->transfertUserData(string);
}

/*void GlobalView::test123()
{
    m_webView->page()->mainFrame()->evaluateJavaScript("test123();");
}*/

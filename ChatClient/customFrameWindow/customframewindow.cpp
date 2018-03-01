#include <QGraphicsDropShadowEffect>
#include "customframewindow.h"
#include <qdebug.h>

CustomFrameWindow::CustomFrameWindow(QWidget *parent): QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

#if defined(Q_OS_WIN)  /* Windows OS */
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
#endif
    setAttribute(Qt::WA_TranslucentBackground);

    setupUi(this);

    restoreButton->setVisible(false);

    /* Fix size of title bar and buttons */
    windowTitlebar->setFixedHeight(22);
    closeButton->setFixedWidth(28);
    maximizeButton->setFixedWidth(28);
    minimizeButton->setFixedWidth(28);
    restoreButton->setFixedWidth(28);

    /* Shadow under window title text */
    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect;
    textShadow->setBlurRadius(4.0);
    textShadow->setColor(QColor(0,0,0));
    textShadow->setOffset(0.0);
    titleText->setGraphicsEffect(textShadow);

    /* Window Shadow */
    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
    windowShadow->setBlurRadius(9.0);
    windowShadow->setColor(Qt::black);
    windowShadow->setOffset(0.0);
    windowFrame->setGraphicsEffect(windowShadow);
}

/* Set what content shall be inside the customFrameWindow */
void CustomFrameWindow::setContent(QWidget *widget)
{
    contentLayout.setMargin(0);
    contentLayout.addWidget(widget);
    windowContent->setLayout(&contentLayout);
}

/* If minimize button is pressed, minimize the window */
void CustomFrameWindow::on_minimizeButton_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

/* If restore button is pressed, make window to restored size */
void CustomFrameWindow::on_restoreButton_clicked()
{
    restoreButton->setVisible(false);
    maximizeButton->setVisible(true);
    setWindowState(Qt::WindowNoState);
    layout()->setMargin(15);
}

/* If maximize button is pressed, make window fullscreen */
void CustomFrameWindow::on_maximizeButton_clicked()
{
    restoreButton->setVisible(true);
    maximizeButton->setVisible(false);
    setWindowState(Qt::WindowMaximized);
    layout()->setMargin(0);
}

/* Close window if close button is pressed */
void CustomFrameWindow::on_closeButton_clicked()
{
    close();
}

/* If the title bar is double clicked, change, depending the current state */
void CustomFrameWindow::on_windowTitlebar_doubleClicked()
{
    if (windowState().testFlag(Qt::WindowNoState))
        on_maximizeButton_clicked();
    else if (windowState().testFlag(Qt::WindowMaximized))
        on_restoreButton_clicked();
}

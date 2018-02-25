#include "customtitlebar.h"

KcWdTitlebar::KcWdTitlebar(QWidget *parent ) :m_parent(parent)
{
    QLabel *title = new QLabel(parent->windowTitle());
    QPushButton *pPB = new QPushButton ("x");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(pPB);

    connect(pPB,SIGNAL(clicked()),parent,SLOT(close()));
}

void KcWdTitlebar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pCursor = event->globalPos() - m_parent->geometry().topLeft();
        event->accept();
    }
}

void KcWdTitlebar::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        m_parent->move(event->globalPos() - m_pCursor);
        event->accept();
    }
}

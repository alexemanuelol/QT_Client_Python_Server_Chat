#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QMouseEvent>

class KcWdTitlebar :public QWidget
{
private:
    QWidget *m_parent;
    QPoint m_pCursor;

public:
    KcWdTitlebar( QWidget *parent) ;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CUSTOMTITLEBAR_H

#ifndef CUSTOMFRAMEWINDOW_H
#define CUSTOMFRAMEWINDOW_H

#include "ui_customframewindow.h"

class CustomFrameWindow: public QWidget, private Ui::CustomFrameWindow
{
    Q_OBJECT

public:
    explicit CustomFrameWindow(QWidget *parent = 0);
    void setContent(QWidget *widget);

private slots:
    void on_minimizeButton_clicked();
    void on_restoreButton_clicked();
    void on_maximizeButton_clicked();
    void on_closeButton_clicked();
    void on_windowTitlebar_doubleClicked();

protected:
    QHBoxLayout contentLayout;
};

#endif // CUSTOMFRAMEWINDOW_H

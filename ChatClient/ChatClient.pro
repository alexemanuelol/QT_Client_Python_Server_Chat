QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH     += "customframewindow"

TARGET          =  ChatClient
TEMPLATE        =  app

SOURCES         += main.cpp\
                   chatclient.cpp\
                   customframewindow/customframewindow.cpp\
                   customframewindow/windowdragger.cpp\

HEADERS         += chatclient.h\
                   customframewindow/customframewindow.h\
                   customframewindow/windowdragger.h\

FORMS           += chatclient.ui\
                   customframewindow/customframewindow.ui

RESOURCES       += res.qrc\
                   customframewindow.qrc\

RC_ICONS        = images/icon.ico

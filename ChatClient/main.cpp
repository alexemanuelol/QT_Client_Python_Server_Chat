#include <QApplication>
#include "customframewindow.h"
#include "chatclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CustomFrameWindow customFrameWindow;

    ChatClient *chatClient = new ChatClient;

    customFrameWindow.setContent(chatClient);
    customFrameWindow.show();

    return a.exec();
}

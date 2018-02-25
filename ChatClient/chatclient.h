#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <qDebug>

namespace Ui {
class ChatClient;
}

class ChatClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatClient(QWidget *parent = 0);
    ~ChatClient();

    void keyPressEvent(QKeyEvent * event);
    void paintEvent(QPaintEvent* event);

private:
    Ui::ChatClient *ui;
    QTcpSocket *socket;
    int PORT_connect;
    QString IP_connect;
    QString message;
    QString messageDC;
    QString server_message;

    QMediaPlayer* messageSound;

    bool isConnected;

private slots:
    void connect_server();
    void disconnect_server();
    void send_text();
    void readFromServer();
};

#endif // CHATCLIENT_H

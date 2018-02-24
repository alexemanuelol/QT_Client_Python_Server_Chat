#include "chatclient.h"
#include "ui_chatclient.h"

ChatClient::ChatClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatClient)
{
    ui->setupUi(this);

    isConnected = false;

    socket = new QTcpSocket(this);

    connect(ui->connect_button, SIGNAL(clicked()), this, SLOT(connect_server()));
    connect(ui->disconnect_button, SIGNAL(clicked()), this, SLOT(disconnect_server()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readFromServer()));
}

ChatClient::~ChatClient()
{
    delete ui;
}

void ChatClient::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return)
        send_text();
}

void ChatClient::connect_server()
{
    IP_connect = ui->IP_line->text();
    PORT_connect = ui->PORT_line->text().split(" ")[0].toInt();

    socket->connectToHost(IP_connect, PORT_connect);

    if (!socket->waitForConnected(100))
        QMessageBox::about(this, "Woops", "Unable to connect to server...");
    else
        isConnected = true;
}

void ChatClient::disconnect_server()
{
    QString messageDC = "disconnect";
    messageDC.append(socket->peerName());
    messageDC.append(":");
    messageDC.append(QString::number(socket->localPort()));

    socket->write(messageDC.toUtf8());

    isConnected = false;
}

void ChatClient::send_text()
{
    message = ui->send_line->text();
    ui->send_line->setText("");

    if (!message.isEmpty() && isConnected)
        socket->write(message.toUtf8());
    else if (message.isEmpty() && isConnected)
        QMessageBox::about(this, "Woops", "You have nothing to send...");
    else
        QMessageBox::about(this, "Woops", "You are not connected to a server...");
}

void ChatClient::readFromServer()
{
    server_message = socket->readAll();

    ui->text_feed->append(server_message);
}

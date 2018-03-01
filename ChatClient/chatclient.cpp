#include "chatclient.h"
#include "ui_chatclient.h"

ChatClient::ChatClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatClient)
{
    ui->setupUi(this);

    errorMessageBox = new QMessageBox;

    /* Set fixed size */
    ui->send_line->setFixedHeight(45);
    ui->PORT_line->setFixedWidth(100);
    ui->nickname->setFixedWidth(100);
    ui->connect_button->setFixedWidth(100);
    ui->connect_button->setFixedHeight(20);
    ui->disconnect_button->setFixedWidth(100);
    ui->disconnect_button->setFixedHeight(20);
    ui->sideWidget->setFixedWidth(120);

    isConnected = false;

    ui->send_line->setDisabled(1);

    socket = new QTcpSocket(this);

    messageSound = new QMediaPlayer();
    messageSound->setMedia(QUrl("qrc:/sounds/res/sound/pop.wav"));
    messageSound->setVolume(50);

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
    if (event->key() == Qt::Key_Return && isConnected)
        send_text();
}

void ChatClient::connect_server()
{
    IP_connect = ui->IP_line->text();
    PORT_connect = ui->PORT_line->text().split(" ")[0].toInt();

    if (!isConnected)
    {
        if (ui->nickname->text() == "")
        {
            messageError = "Please enter a nickname!";
            errorMessageBox->critical(this, "Error", messageError);
        }
        else if (ui->nickname->text().contains("~"))
        {
            messageError = "Invalid letter '~' in nickname\nPlease try again!";
            errorMessageBox->critical(this, "Error", messageError);
        }
        else
        {
            socket->connectToHost(IP_connect, PORT_connect);

            if (!socket->waitForConnected(100))
            {
                messageError = "Unable to connect to:\nIP:         ";
                messageError.append(IP_connect);
                messageError.append("\nPORT:   ");
                messageError.append(QString::number(PORT_connect));
                errorMessageBox->critical(this, "Error", messageError);
            }
            else
            {
                isConnected = true;

                nickname = ui->nickname->text();
                message = nickname;
                message.append(" is now online!");
                socket->write(message.toUtf8());

                nickname = ui->nickname->text();
                ui->send_line->setPlaceholderText("Enter message here");
                ui->send_line->setEnabled(1);
                ui->IP_line->setDisabled(1);
                ui->PORT_line->setDisabled(1);
                ui->nickname->setDisabled(1);
            }
        }
    }
}

void ChatClient::disconnect_server()
{
    if (isConnected)
    {
        messageDC = "disconnect";
        messageDC.append(socket->peerName());
        messageDC.append(":");
        messageDC.append(QString::number(socket->localPort()));
        messageDC.append(nickname);

        socket->write(messageDC.toUtf8());

        isConnected = false;
        ui->send_line->setDisabled(1);
        ui->send_line->setText("");
        ui->send_line->setPlaceholderText("");
        ui->IP_line->setEnabled(1);
        ui->PORT_line->setEnabled(1);
        ui->nickname->setEnabled(1);
    }
    else
        errorMessageBox->critical(this, "Error", "You are not connected to a server...");
}

void ChatClient::send_text()
{   
    message = ui->send_line->text();

    if (message.isEmpty())
        errorMessageBox->critical(this, "Error", "You have nothing to send...");
    else if (!message.isEmpty() && !message.contains("~"))
    {
        message = nickname;
        message.append(": ");
        message.append(ui->send_line->text());
        ui->send_line->setText("");
        socket->write(message.toUtf8());
    }
}

void ChatClient::readFromServer()
{
    if (messageSound->state() == QMediaPlayer::PlayingState)
        messageSound->setPosition(0);
    else if (messageSound->state() == QMediaPlayer::StoppedState)
        messageSound->play();

    server_message = socket->readAll();

    if (server_message.contains(" is now online!"))
    {
        server_message.remove(" is now online!");
        server_message.remove(0, 8);
        server_message.append(" online");
        ui->users_online->append(server_message);
    }
    else if (server_message.contains(" has gone offline!"))
    {
        server_message.remove(" has gone offline!");
        server_message.remove(0, 8);
        server_message.append(" offline");
        ui->users_online->append(server_message);
    }
    else
    {
        ui->text_feed->append(server_message);
    }
}

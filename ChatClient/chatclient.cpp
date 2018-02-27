#include "chatclient.h"
#include "ui_chatclient.h"

ChatClient::ChatClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatClient)
{
    ui->setupUi(this);

    errorMessageBox = new QMessageBox;

    /* Set background color */
    this->setStyleSheet("ChatClient {background: QColor(66,0,73);}");

    /* Customize the text fields */
    ui->text_feed->setStyleSheet("QTextBrowser {border: 0px;background: QColor(66, 0, 73); font-family: Whitney; font-style: normal; font-size: 10pt; color: white;}");
    ui->send_line->setStyleSheet("QLineEdit {border: 0px;border-radius: 3px; padding: 0 8px;background: QColor(78, 0, 104); font-family: Whitney; font-style: normal; font-size: 10pt; color: white;}");

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
        ui->send_line->setPlaceholderText("Enter message here");
        ui->send_line->setEnabled(1);
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

        socket->write(messageDC.toUtf8());

        isConnected = false;
        ui->send_line->setDisabled(1);
        ui->send_line->setText("");
        ui->send_line->setPlaceholderText("");
    }
    else
        errorMessageBox->critical(this, "Error", "You are not connected to a server...");
}

void ChatClient::send_text()
{   
    message = ui->send_line->text();
    ui->send_line->setText("");

    if (!message.isEmpty() && isConnected)
        socket->write(message.toUtf8());
    else if (message.isEmpty() && isConnected)
        errorMessageBox->critical(this, "Error", "You have nothing to send...");
}

void ChatClient::readFromServer()
{
    if (messageSound->state() == QMediaPlayer::PlayingState)
        messageSound->setPosition(0);
    else if (messageSound->state() == QMediaPlayer::StoppedState)
        messageSound->play();

    server_message = socket->readAll();
    ui->text_feed->append(server_message);
}

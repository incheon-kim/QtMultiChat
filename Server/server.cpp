#include "server.h"
#include <QString>
#include <QRegExp>

Server::Server(QObject* parent) : QObject(parent) {
    server = new QTcpServer(this);
    //manager=new Roommanager();
    connect(server, SIGNAL(newConnection()),
            this,   SLOT(onNewConnection()));

    if (!server->listen(QHostAddress::Any, PORT)) {
        qDebug() << "Server is not started.";
    } else {
        qDebug() << "Server is started.";
    }
}

/* Отправить всем обновленный список пользователей */
void Server::sendUserList()
{
    QString line = "/users:" + clients.values().join(',') + "\n";
    sendToAll(line);
}

/* Отправить сообщение всем пользователям */
void Server::sendToAll(const QString& msg) {
    foreach (QTcpSocket* socket, clients.keys()) {
        socket->write(msg.toUtf8());
    }
}

/* Слот, который вызывается, когда к серверу подключается
 * новый клиент */
void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    QString number; //????????
    if(room[room_Pointer] <= 1)//room not full
    {
        ++room[room_Pointer];
        number = "number";
    }
    else //room full (need to modify later)
    {
        ++room[++room_Pointer];
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    // оставим клиента безымянным пока он не залогинится
    clients.insert(socket, "");
    QString setClientRoomNumber = QString::number(room_Pointer); //tmp is client's room_no
    sendToAll(QString(number + ":" + setClientRoomNumber + "\n")); //send to connected client
}

/* Слот, вызываемый при отключении клиента */
void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();

    QString username = clients.value(socket);
    sendToAll(QString("/system:" + username + " has left the chat.\n"));
    clients.remove(socket);
    sendUserList();
}

/* Прием данных от клиента */
void Server::onReadyRead() {
    QRegExp loginRex("^/login:(.*)$");
    QRegExp messageRex("^(.*):/say:(.*)$");
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        /* Сообщение - пользователь логинится */
        if (loginRex.indexIn(line) != -1) {
            QString user = loginRex.cap(1);
            clients[socket] = user;
            sendToAll(QString("/system:" + user + " has joined the chat.\n"));
            sendUserList();
            qDebug() << user << "logged in.";
        }
        /* Сообщение в чат */
        else if (messageRex.indexIn(line) != -1) {
            QString user = clients.value(socket);
            QString num = messageRex.cap(1);
            qDebug() << "넘버링 테스트: "<<num;
            QString msg = messageRex.cap(2);
            sendToAll(QString(num + ":" + user + ":" + msg + "\n"));
            qDebug() << "User:" << user;
            qDebug() << "Message:" << msg;
        }
        /* Некорректное сообщение от клиента */
        else {
            qDebug() << "Bad message from " << socket->peerAddress().toString();
        }
    }
}


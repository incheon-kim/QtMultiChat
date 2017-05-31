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


void Server::sendUserList()
{
    QString line = "/users:" + clients.values().join(',') + "\n";
    sendToAll(line);
}


void Server::sendToAll(const QString& msg) {
    foreach (QTcpSocket* socket, clients.keys()) {
        socket->write(msg.toUtf8());
    }
}


void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    QString number;
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


void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();

    QString username = clients.value(socket);
    sendToAll(QString("/system:" + username + " has left the chat.\n"));
    clients.remove(socket);
    sendUserList();
}


void Server::onReadyRead() {
    QRegExp loginRex("^/login:(.*)$");
    QRegExp messageRex("^(.*):/say:(.*)$");
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        if (loginRex.indexIn(line) != -1) {
            QString user = loginRex.cap(1);
            clients[socket] = user;
            sendToAll(QString("/system:" + user + " has joined the chat.\n"));
            sendUserList();
            qDebug() << user << "logged in.";
        }

        else if (messageRex.indexIn(line) != -1) {
            QString user = clients.value(socket);
            QString num = messageRex.cap(1);
            qDebug() << "넘버링 테스트: "<<num;
            QString msg = messageRex.cap(2);
            sendToAll(QString(num + ":" + user + ":" + msg + "\n"));
            qDebug() << "User:" << user;
            qDebug() << "Message:" << msg;
        }

        else {
            qDebug() << "Bad message from " << socket->peerAddress().toString();
        }
    }
}


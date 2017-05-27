#include "server.h"
#include <QString>
#include <QRegExp>
#include <QtCore/QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtDebug>

Server::Server(QObject* parent) : QObject(parent) {
    this->crypto.setKey(0x0c2ad4a4acb9f023);
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),
            this,   SLOT(onNewConnection()));

    if (!server->listen(QHostAddress::Any, PORT)) {
        qDebug() << "Server is not started.";
    } else {
        qDebug() << "Server is started.";
    }
}


void Server::sendUserList() {
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
    qDebug() << "Client connected: " << socket->peerAddress().toString();

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));


    clients.insert(socket, "");
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
    QRegExp signupRex("^/makeID:(.*)/makepw:(.*)/makeemail:(.*)/makegender:([0-1])$");
    QRegExp loginRex("^/login:(.*)$");
    QRegExp messageRex("^/say:(.*)$");
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
            QString msg = messageRex.cap(1);
            sendToAll(QString(user + ":" + msg + "\n"));
            qDebug() << "User:" << user;
            qDebug() << "Message:" << msg;
        }

        else if(signupRex.indexIn(line)!=-1){
            QString id=signupRex.cap(1);
            QString enpw=signupRex.cap(2);
            QString dcpw=crypto.decryptToString(enpw);
            QString email=signupRex.cap(3);
            QString gender=signupRex.cap(4);

        }

        else {
            qDebug() << "Bad message from " << socket->peerAddress().toString();
        }
    }
}


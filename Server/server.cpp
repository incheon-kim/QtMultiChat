#include "server.h"
#include <QString>
#include <QRegExp>


Server::Server(QObject* parent) : QObject(parent) {
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),
            this,   SLOT(onNewConnection()));

    if (!server->listen(QHostAddress::Any, PORT)) {
        qDebug() << "Server is not started.";
    } else {
        qDebug() << "Server is started.";
    }
}

/* 모든 사용자의 업데이트된 목록을 보내기 */
void Server::sendUserList() {
    QString line = "/users:" + clients.values().join(',') + "\n";
    sendToAll(line);
}

/* 모든 사용자에게 메시지 보내기 */
void Server::sendToAll(const QString& msg) {
    foreach (QTcpSocket* socket, 기clients.keys()) {
        socket->write(msg.toUtf8());
    }
}

/* 서버에 연결될 때 호출되는 슬롯
 * 새로운 클라이언트 */
void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    qDebug() << "Client connected: " << socket->peerAddress().toString();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    // 그는 로그인되지 않은 상태에서 익명의 클라이언트를 남겨
    clients.insert(socket, "");
}

/* 클라이언트 연결이 해제 될 때 호출되는 슬롯 */
void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();

    QString username = clients.value(socket);
    sendToAll(QString("/system:" + username + " has left the chat.\n"));
    clients.remove(socket);
    sendUserList();
}

/*클라이언트에서 데이터 수신*/
void Server::onReadyRead() {
    QRegExp loginRex("^/login:(.*)$");
    QRegExp messageRex("^/say:(.*)$");
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        /* 메시지 - 사용자 로그 */
        if (loginRex.indexIn(line) != -1) {
            QString user = loginRex.cap(1);
            clients[socket] = user;
            sendToAll(QString("/system:" + user + " has joined the chat.\n"));
            sendUserList();
            qDebug() << user << "logged in.";
        }
        /* 채팅 메시지 */
        else if (messageRex.indexIn(line) != -1) {
            QString user = clients.value(socket);
            QString msg = messageRex.cap(1);
            sendToAll(QString(user + ":" + msg + "\n"));
            qDebug() << "User:" << user;
            qDebug() << "Message:" << msg;
        }
        /* 클라이언트에서 잘못된 메시지 */
        else {
            qDebug() << "Bad message from " << socket->peerAddress().toString();
        }
    }
}

void Server::pushClientQueue(QTcpSocket *socket){
    clientQueue.enqueue(socket);
    size++; //큐 사이즈
}

QTcpSocket* Server::popCilentQueue(){
    return clientQueue.dequeue();
}

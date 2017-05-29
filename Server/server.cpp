#include "server.h"
#include <QString>
#include <QRegExp>

Server::Server(QObject* parent) : QObject(parent) {
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),
            this,   SLOT(onNewConnection()));

    r=new RoomManager();

    if (!server->listen(QHostAddress::Any, PORT)) 
    {
        qDebug() << "Server is not started.";
    } 
    else 
    {
        qDebug() << "Server is started.";
    }
}

void Server::sendUserList() 
{
    QString line = "/users:" + clients.values().join(',') + "\n";
    //sendToAll(line);
}

/*
//Отправить сообщение всем пользователям
void Server::sendToTarget(const QString& msg, QTcpSocket* sender) 
{
    GameRoom* TargetRoom;
    qDebug()<<"send start";
    for(int i=0; i<userList.count();i++)
    {
        if(userList[i].getSocket()==sender){
            TargetRoom = userList[i].room;
            break;
        }
    }
    for(int i=0; i<userList.count();i++){
        if(userList[i].room == TargetRoom){
            userList[i].getSocket()->write(msg.toUtf8());
        }
    }
}
*/
void Server::sendToAll(const QString& msg){
    foreach (QTcpSocket* socket, clients.keys()) {

            socket->write(msg.toUtf8());
        }
}

/* Слот, который вызывается, когда к серверу подключается
 * новый клиент */
void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    qDebug() << "Client connected: " << socket->peerAddress().toString();

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    // оставим клиента безымянным пока он не залогинится
    GameUser newUser(socket);
    userList.enqueue(newUser);
    qDebug()<<userList.count();
    userNumber++;
    qDebug()<<"user added";
}

/* Слот, вызываемый при отключении клиента */
void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();

    QString username = clients.value(socket);
    //sendToAll(QString("/system:" + username + " has left the chat.\n"));
    clients.remove(socket);
    sendUserList();
}

/* Прием данных от клиента */
void Server::onReadyRead() {
    GameRoom* room;
    qDebug()<<"not started pop";
    qDebug()<<sizeUserList();
    if(sizeUserList()>=2)
    {
        qDebug()<<"pop started";
        GameUser player1 = userList.dequeue();
        GameUser player2 = userList.dequeue();
        //userList.dequeue();

        room = r->createRoom();

        player1.enterRoom(room);
        player2.enterRoom(room);

        room->sendToTarget(QString("room make"),player1.getSocket());
    }

    QRegExp loginRex("^/login:(.*)$");
    QRegExp messageRex("^/say:(.*)$");
    QTcpSocket* socket = (QTcpSocket*)sender();

    while (socket->canReadLine()) 
    {

        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        // Сообщение - пользователь логинится
        if (loginRex.indexIn(line) != -1) {
            QString user = loginRex.cap(1);
            clients[socket] = user;
            //qDebug()<<"test";
            //sendToTarget(QString("Hello"),socket);
            sendToAll(QString("/system:" + user + " has joined the chat.\n"));
            //sendUserList();
            qDebug() << user << "logged in.";
        }
        // Сообщение в чат
        else if (messageRex.indexIn(line) != -1) {
            QString user = clients.value(socket);
            QString msg = messageRex.cap(1);
            room->sendToTarget(QString("msggggggggggggg"),socket); //new added
            qDebug()<<"VERIFY "<<room;
            sendToAll(QString(user + ":" + msg + "\n"));
            qDebug() << "User:" << user;
            qDebug() << "Message:" << msg;
        }
        //Некорректное сообщение от клиента
        else {
            qDebug() << "Bad message from " << socket->peerAddress().toString();
        }
    }

}

int Server::sizeUserList()
{
    int size;
    size=userList.count();
    return size;
}


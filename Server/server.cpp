#include "server.h"
#include <QString>
#include <QRegExp>

Server::Server(QObject* parent) : QObject(parent) {
    server = new QTcpServer(this);
    manager=new Roommanager();
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
    bool enter=false;
    int room_number=1;
    QVector<Room>::iterator iter;
    if(!manager->emptyRooms())
    {
        qDebug()<<"not empty";
        for(iter =manager->returnIteratorBegin();iter!=manager->returnIteratorEnd();++iter)
        {
        qDebug()<<"test";
        qDebug()<<"getNumber"<<(*iter).getNumOfPeople();
            if((*iter).getNumOfPeople()<1)
            {
            (*iter).enter();
            number="number";
            enter=true;
            break;
            }
        room_number++;
        }
    }

    qDebug()<<"roomnumber"<<room_number;
    if(enter==false)
    {
        Room newRoom;
       manager->createRoom(newRoom);
        Room createRoom =manager->getLastRoom();
        createRoom.enter();
    }
    /*
    if(room[room_Pointer] <= 1)//room not full
    {
        ++room[room_Pointer];
        number = "number";
    }
    else //room full (need to modify later)
    {
        ++room[++room_Pointer];
    }
    */

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    clients.insert(socket, "");
    QString setClientRoomNumber = QString::number(room_number);
    //QString setClientRoomNumber = QString::number(room_Pointer); //tmp is client's room_no
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


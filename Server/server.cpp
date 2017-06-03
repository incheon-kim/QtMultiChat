#include "server.h"
#include <QString>
#include <QRegExp>

Server::Server(QObject* parent) : QObject(parent) {
    server = new QTcpServer(this);
    manager=new RoomManager();
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
    //????dont' know how to handle values
    //QString line = "/users:" + clients.values().join(',') + "\n";
    //sendToAll(line);
}

void Server::sendToAll(const QString& msg) {
    foreach (QTcpSocket* socket, clients.keys()) {
        socket->write(msg.toUtf8());
    }
}

void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    QString number;
    int roomNumber=1;
    bool enter=false;
     QVector<Room>::iterator iter;

    if(!manager->isEmpty())
    {

        for(iter=manager->beginIterator();iter!=manager->endItertor();++iter)
        {
            if((*iter).getPeople()<2) //need to modify here later(male/female)
            {
                (*iter).enter();
                number="number";
                enter=true;
                qDebug()<<"roomPeople"<<(*iter).getPeople();
                qDebug()<<"roomNumber"<<roomNumber;
                break;
            }
            roomNumber++;
        }
    }

    if(enter==false)
    {

        manager->createRoom();
        QVector<Room>::iterator getRoom= manager->lastIter();
        (*getRoom).enter();
        qDebug()<<"roomPeople"<<(*getRoom).getPeople();
        qDebug()<<"enterRoom"<<roomNumber;
    }


    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    userInfo info;
    info.roomNumber=roomNumber;
    clients.insert(socket, info);


    QString setClientRoomNumber = QString::number(roomNumber);
    sendToAll(QString(number + ":" + setClientRoomNumber + "\n")); //send to connected client
}

void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();

    int compareRoomNumber=1;
     QVector<Room>::iterator iter;
     userInfo temp;
     temp=clients[socket];

     if(!manager->isEmpty())
     {

         for(iter=manager->beginIterator();iter!=manager->endItertor();++iter)
         {
             if(compareRoomNumber==temp.roomNumber)
             {
                 (*iter).out();
                 qDebug()<<"roomPeople"<<(*iter).getPeople();
                 qDebug()<<"roomNumber"<<temp.roomNumber;
                 break;
             }
             compareRoomNumber++;
      }


    QString username = clients.value(socket).userName;
    sendToAll(QString("/system:" + username + " has left the chat.\n"));
    clients.remove(socket);
    sendUserList();
}
}

void Server::onReadyRead() {
    QRegExp loginRex("^/login:(.*)$");
    QRegExp messageRex("^(.*):/say:(.*)$");
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        if (loginRex.indexIn(line) != -1) {
            QString user = loginRex.cap(1);
            userInfo temp;
            temp=clients[socket];
            temp.userName=user;
            clients[socket] = temp;
            sendToAll(QString("/system:" + user + " has joined the chat.\n"));
            sendUserList();
            qDebug() << user << "logged in.";
        }

        else if (messageRex.indexIn(line) != -1) {
            QString user = clients.value(socket).userName;
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


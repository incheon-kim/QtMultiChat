#include "userinfo.h"
#include <QTcpSocket>
#include <QList>
UserInfo::UserInfo()
{

}

void UserInfo::insert_user(QTcpSocket* socket, QString str){
    clients.insert(socket, str);
}

QString UserInfo::getUserName(QTcpSocket* socket){
    return clients.value(socket);
}

QList<QTcpSocket*> UserInfo::getUserKeys(){
    return clients.keys();
}

QList<QString> UserInfo::getValues(){
    return clients.values();
}

const QString UserInfo::getValue(QTcpSocket* socket){
    return clients.value(socket);
}

int UserInfo::removeUser(QTcpSocket* socket){
    return clients.remove(socket);
}

void UserInfo::setUserName(QTcpSocket* socket, QString name){
    clients[socket] = name;
}



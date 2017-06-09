#include "user.h"

User::User()
{
}
User::User(QTcpSocket*socket)
{
    this->clientRoomNumber=0;
    this->socket=socket;
}

void User::setRoomNumber(int clientRoomNumber)
{
    this->clientRoomNumber=clientRoomNumber;
}

int User::getRoomNumber()
{
    return clientRoomNumber;
}

QTcpSocket* User::getSocket()
{
    return socket;
}

void User::setUserID(QString name){
    this->userID = name;
}

QString User::getUserID(){
     return this->userID;
}

void User::setLoginFlag(bool flag){
    this->loginFlag = flag;
}

bool User::getLoginFlag(){
    return this->loginFlag;
}

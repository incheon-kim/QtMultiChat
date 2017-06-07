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

#ifndef GAMEUSER_H
#define GAMEUSER_H
#include "gameroom.h"
#include <QTcpSocket>
class GameRoom;
class GameUser
{

private:

    QTcpSocket* userSocket;
public:
    GameUser(QTcpSocket* userSocket)
    {
        this->userSocket=userSocket;
    }


    void enterRoom(GameRoom *room);
    QTcpSocket* getSocket();
    GameRoom *room; //room that user is in

};

#endif // GAMEUSER_H

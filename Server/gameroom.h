#ifndef GAMEROOM_H
#define GAMEROOM_H
#include "gameuser.h"
#include "server.h"
#include "QList"
class GameUser;
class GameRoom
{
public:
    GameRoom();
    void enterRoom(GameUser enterUser);
    //void exitRoom(GameUser exitUser);
    int SizeofUser();
    void sendToTarget(const QString& msg, QTcpSocket* sender);

private:
    QList<GameUser> userList;

};

#endif // GAMEROOM_H

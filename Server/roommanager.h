#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include "gameroom.h"
#include <QList>
class GameRoom;
class RoomManager
{
public:
    RoomManager();
    GameRoom* createRoom();
    void RemoveRoom();
    int RoomCount();

private:
    QList<GameRoom> roomList;



};

#endif // ROOMMANAGER_H

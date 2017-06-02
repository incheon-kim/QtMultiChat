#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include <QVector>
#include "room.h"
class Room;
class RoomManager
{
public:
    RoomManager();
    void createRoom();
    //void setDisconnect();
    bool isEmpty();
    QVector<Room>::iterator lastIter();
    QVector<Room>::iterator beginIterator();
    QVector<Room>::iterator endItertor();

private:
    QVector<Room> roomList;
    //bool disconnect;
};

#endif // ROOMMANAGER_H

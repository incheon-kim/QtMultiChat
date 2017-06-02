#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include <QVector>
#include "room.h"
class Room;
class Roommanager
{
public:
    Roommanager();
    Room  getLastRoom();
    void createRoom(Room newRoom);
    QVector<Room>::iterator returnIteratorBegin();
    QVector<Room>::iterator returnIteratorEnd();
    bool emptyRooms();
private:
   QVector<Room> roomList;
};

#endif // ROOMMANAGER_H

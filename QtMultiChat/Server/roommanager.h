#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H
#include <QVector>
#include "room.h"
class Room;
class Roommanager
{
public:
    Roommanager();
    void getRoom(QVector<Room>::iterator iter);
    void createRoom();
private:
   QVector<Room> roomList;
};

#endif // ROOMMANAGER_H

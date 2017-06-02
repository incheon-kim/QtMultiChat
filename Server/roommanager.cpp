#include "roommanager.h"

Roommanager::Roommanager()
{
}
void Roommanager::createRoom(Room newRoom)
{
   roomList.append(newRoom);
}

 Room Roommanager::getLastRoom()
 {
     return roomList.last();
 }

 QVector<Room>::iterator Roommanager::returnIteratorBegin()
 {
     return roomList.begin();
 }
 QVector<Room>::iterator Roommanager::returnIteratorEnd()
 {
     return roomList.end();
 }

bool Roommanager::emptyRooms()
{
    if(roomList.empty())
        return true;
    else
        return false;
}

#include "roommanager.h"

RoomManager::RoomManager()
{
    //disconnect=false;

}

bool RoomManager::isEmpty()
{
    if(roomList.empty())
        return true;
    else
        return false;
}

void RoomManager::createRoom()
{
    Room newRoom;
    roomList.push_back(newRoom);
}

QVector<Room>::iterator RoomManager::lastIter()
{
    QVector<Room>::iterator iter = roomList.end();
    --iter;
    return iter;
}

QVector<Room>::iterator RoomManager::beginIterator()
{
    return roomList.begin();
}
QVector<Room>::iterator RoomManager::endItertor()
{
    return roomList.end();
}
/*
void RoomManager::setDisconnect()
{
    disconnect=true;
}*/

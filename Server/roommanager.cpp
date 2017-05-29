#include "roommanager.h"


RoomManager::RoomManager()
{
}

GameRoom* RoomManager::createRoom()
{
    GameRoom *room = new GameRoom();
    roomList.append(*room);
    qDebug()<<"making room";
    return room;
}

void RoomManager::RemoveRoom()
{
    int rmRoom;
    for(int i=0;i<sizeof(roomList);i++){
        if(roomList[i].SizeofUser()<2)
            rmRoom = i;
    }
    roomList.removeAt(rmRoom); //need to verify
    qDebug()<<"remove room";
}

int RoomManager::RoomCount()
{
    return roomList.size();
}

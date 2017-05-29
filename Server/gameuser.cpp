#include "gameuser.h"
\


void GameUser::enterRoom(GameRoom *room)
{
    this->room=room;
    room->enterRoom(*this);

}
QTcpSocket* GameUser::getSocket(){
    return this->userSocket;
}

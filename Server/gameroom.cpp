#include "gameroom.h"
#include "roommanager.h"

GameRoom::GameRoom()
{

}
int GameRoom::SizeofUser(){
    int size;
    size = sizeof(this->userList);

    return size;
}
void GameRoom::sendToTarget(const QString& msg, QTcpSocket* sender)
{
    //finding room complete
    GameRoom* TargetRoom;
    qDebug()<<"send start";
    for(int i=0; i<userList.count();i++){
        qDebug()<<userList.count();
        if(userList[i].getSocket()==sender){
            qDebug()<<"Enter";
            TargetRoom = userList[i].room;
            break;
        }
    }

     //

    //userList[0].getSocket()->write(msg.toUtf8());
    //qDebug() <<"user 0 write";
    //userList[1].getSocket()->write(msg.toUtf8());
    //qDebug()<<"user 1 write";

    for(int i=0; i<userList.count();i++){
        qDebug()<<i<<"TT";
        if(userList[i].room == TargetRoom){
            qDebug()<<"Room";
            userList[i].getSocket()->write(msg.toUtf8());
            qDebug()<<"ehoTsldksehoTsl";
        }
    }
}

void GameRoom::enterRoom(GameUser enterUser)
{
   userList.append(enterUser);
}

/*void GameRoom::exitRoom(GameUser exitUser)
{
    userList.removeAll(exitUser);
    ->RemoveRoom(this);

}*/

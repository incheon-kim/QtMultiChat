#include "room.h"
#include <QDebug>
Room::Room()
{
    numOfPeople=0;
}

int Room::getNumOfPeople()
{
    return numOfPeople;
}

void Room::enter()
{
    qDebug()<<"beforeenter"<<numOfPeople;
    numOfPeople++;
    qDebug()<<"new enter"<<numOfPeople;
}

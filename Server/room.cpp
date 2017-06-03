#include "room.h"

Room::Room()
{
   //female=false;
   // male=fale;
    numPeople=0;
}

void Room::enter()
{
    ++numPeople;
}
int Room::getPeople()
{
    return numPeople;
}
void Room::out()
{
    --numPeople;
}
/*
bool Room::getFemale()
{
    if(female==true)
        return true;
    else
        return false;
}
bool Room::getMale()
{
    if(male==true)
        return true;
    else return false;
}
void Room::enterFemale()
{
    female=true;
}
void Room::enterMale()
{
    male=true;
}*/

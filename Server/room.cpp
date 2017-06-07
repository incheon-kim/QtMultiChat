#include "room.h"

Room::Room()
{
   female=false;
   male=false;
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

bool Room::needFemale()
{
    if(female==false)
        return true;
    else
        return false;
}
bool Room::needMale()
{
    if(male==false)
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
}
void Room::outFemale()
{
    female=false;
}
void Room::outMale()
{
    male=false;
}

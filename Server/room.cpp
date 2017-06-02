#include "room.h"

Room::Room()
{
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

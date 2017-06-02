#ifndef ROOM_H
#define ROOM_H


class Room
{
public:
    Room();
    void enter();
    int getPeople();
private:
    int numPeople;
};

#endif // ROOM_H

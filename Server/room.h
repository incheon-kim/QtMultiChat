#ifndef ROOM_H
#define ROOM_H


class Room
{
public:
    Room();
    void enter();
    void out();
    int getPeople();
    /*
    bool getFemale();
    bool getMale();
    void enterFemale();
    void enterMale();
    */
private:
    int numPeople;
   // bool male;
   // bool female;
};

#endif // ROOM_H

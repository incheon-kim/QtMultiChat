#ifndef USER_H
#define USER_H
#include <QTcpSocket>

class User
{
public:
    User();
    User(QTcpSocket*socket);
    void setRoomNumber(int clientRoomNumber);
    int getRoomNumber();
     QTcpSocket* getSocket();
private:
    QTcpSocket* socket;
    int clientRoomNumber; //client's own number;
};

#endif // USER_H

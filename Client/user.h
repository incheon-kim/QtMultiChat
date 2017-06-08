#ifndef USER_H
#define USER_H
#include <QTcpSocket>
#include <QString>

class User
{
public:
    User();
    User(QTcpSocket *socket);
    void setRoomNumber(int clientRoomNumber);
    int getRoomNumber();
    QTcpSocket *getSocket();
    void setUserID(QString name);
    QString getUserID();
private:
    QTcpSocket *socket;
    int clientRoomNumber; //client's own number;
    QString userID;
};

#endif // USER_H

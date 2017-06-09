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
    void setLoginFlag(bool flag);
    bool getLoginFlag();
private:
    QTcpSocket *socket;
    int clientRoomNumber; //client's own number;
    QString userID;
    bool loginFlag = false;
};

#endif // USER_H

#ifndef USER_H
#define USER_H
#include <QTcpSocket>
#include <QString>

class User
{
public:
    User();
    User(QTcpSocket *socket);

    // SETTER
    void setUserID(QString name);
    void setRoomNumber(int clientRoomNumber);
    void setLoginFlag(bool flag);
    // GETTER
    int getRoomNumber();
    QTcpSocket *getSocket();
    QString getUserID();
    bool getLoginFlag();
private:
    QTcpSocket *socket;
    QString userID;
    int clientRoomNumber; //client's own number;
    bool loginFlag = false;
};

#endif // USER_H

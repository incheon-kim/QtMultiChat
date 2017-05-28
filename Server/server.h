#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QQueue>
#include <QVector>
#include "userinfo.h"
#define PORT 1234


class Server : QObject {
    Q_OBJECT
public:
    explicit Server(QObject* parent = 0);
    void sendUserList();
    void sendToAll(const QString&);

public slots:
    void onNewConnection();
    void onDisconnect();
    void onReadyRead();
    int checkUserNumber();
    void makeRoom();
private:
    QTcpServer* server;
    UserInfo userInfo;
    QQueue<UserInfo> clientQueue;
    int userNumber = 0;
    //QVector<UserInfo> curUser;//현재 유저의 정보 저장
    int roomNumber = 0;

};

#endif // SERVER_H

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
#include "roommanager.h"
#define PORT 1234
class RoomManager;
class GameUser;
class Server : QObject {
    Q_OBJECT
public:
    explicit Server(QObject* parent = 0);
    void sendUserList();
    void sendToTarget(const QString& msg, QTcpSocket* sender);
    void sendToAll(const QString& msg);
    int sizeUserList();
public slots:
    void onNewConnection();
    void onDisconnect();
    void onReadyRead();
private:
    RoomManager *r;
    QTcpServer* server;
    QMap<QTcpSocket*,QString> clients;
    QQueue<GameUser> userList; //all user
    int userNumber;
};

#endif //


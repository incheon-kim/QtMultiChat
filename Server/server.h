#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include "roommanager.h"
#include "room.h"
#include <QtSql/QSqlDatabase>
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include "server.h"
#include "simplecrypt.h"
#define PORT 1234


class Server : QObject {
    Q_OBJECT
public:
    explicit Server(QObject* parent = 0);
    void sendUserList();
    void sendToAll(const QString&);
    ~Server();
public slots:
    void onNewConnection();
    void onDisconnect();
    void onReadyRead();
private:
    QTcpServer* server;
    QMap<QTcpSocket*,QString> clients;
    int room[10] = {0};
    int room_Pointer = 1;
    int room_Current = 0; //allocated room
    //Roommanager *manager;
    QSqlDatabase myDB;
    SimpleCrypt crypto;
};

#endif // SERVER_H

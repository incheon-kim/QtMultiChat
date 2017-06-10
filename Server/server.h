#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QStringList>
#include <QRegExp>

#include <QMap>
#include <QVector>

#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFileInfo>

#include "server.h"
#include "simplecrypt.h"
#include "smtp.h"
#include "roommanager.h"
#include "room.h"

#define PORT 1234

typedef struct
{
    QString userName ="NONE";
    int userSex =-1;
    int roomNumber=-1;
}userInfo;
//man 0 female 1

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
    QMap<QTcpSocket*,userInfo> clients;
    RoomManager *manager;
    QSqlDatabase myDB;
    SimpleCrypt crypto;

private slots:
    void sendMail(QString, QString);
    void mailSent(QString);
};

#endif // SERVER_H

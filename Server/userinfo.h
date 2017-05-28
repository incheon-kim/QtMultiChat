#ifndef USERINFO_H
#define USERINFO_H
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QMap>
class UserInfo
{
private:
    QMap<QTcpSocket*, QString> clients;
public:
    UserInfo();
    void insert_user(QTcpSocket* clients, QString str); //유저를 큐에 삽입
    QString getUserName(QTcpSocket* socket); //유저의 이름을 리턴
    QList<QTcpSocket*> getUserKeys(); //모든 유저의 키를 리턴
    QList<QString> getValues(); //모든 유저의 밸류를 리턴
    const QString getValue(QTcpSocket* socket);
    int removeUser(QTcpSocket* socket);
    void setUserName(QTcpSocket* socket, QString name);
};

#endif // USERINFO_H

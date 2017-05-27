#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QtSql/QSqlDatabase>
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
class DbManager
{
public:
    DbManager(const QString& path);
    bool addPerson(const QString& id,const QString& pw,const QString& email,const QString& gender);
    bool checkLogin(const QString& id,const QString& pw);
private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H

#include "dbmanager.h"

DbManager::DbManager(const QString &path)
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName(path);
    if(!m_db.open()){
        qDebug()<<"Error: connection with database failed";
    }
    else{
        qDebug()<<"Database: connection ok";
    }

}

bool DbManager::addPerson(const QString& id,const QString& pw,const QString& email,const QString& gender,const QString& token){
    QSqlQuery query;
    bool success;
    query.prepare(QString("INSERT INTO info (ID, PW, Email,Enable,Gender) VALUES (:ID, :PW, :Email, :Enable, :Gender)"));
    query.bindValue(":ID",id);
    query.bindValue(":PW",pw);
    query.bindValue(":Email",email);
    query.bindValue(":Enable",'1');
    query.bindValue(":Gender",gender);

    if(query.exec())
      {
          success = true;
      }
      else
      {
           qDebug() << "addPerson error:"
                    << query.lastError().text();
           return false;
      }

      return success;

}

bool DbManager::checkLogin(const QString& id,const QString& pw){
    QSqlQuery query;
    query.prepare("SELECT ID,PW FROM INFO WHERE ID = (:id), PW = (:pw);");
    query.bindValue(":id", id);
    query.bindValue(":pw",pw);

    if (query.exec())
    {
       if (query.next())
       {
          // it exists
       }
    }

}

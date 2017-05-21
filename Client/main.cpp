#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString servername = "LOCALHOST\\SQLITE";
    QString dbname = " /home/kim/dongdongju";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setConnectOptions();
    db.setDatabaseName(dbname);
    if(!db.open()){
         qDebug()<<"Error"<<db.lastError().text();

     }
    w.show();

    return a.exec();
}

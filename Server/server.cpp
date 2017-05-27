#include "server.h"
#include <QString>
#include <QRegExp>
#include <QtCore/QCoreApplication>

Server::Server(QObject* parent) : QObject(parent) {
    this->crypto.setKey(0x0c2ad4a4acb9f023);
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()),
            this,   SLOT(onNewConnection()));

    if (!server->listen(QHostAddress::Any, PORT)) {
        qDebug() << "Server is not started.";
    } else {
        qDebug() << "Server is started.";
    }
}


void Server::sendUserList() {
    QString line = "/users:" + clients.values().join(',') + "\n";
    sendToAll(line);
}


void Server::sendToAll(const QString& msg) {
    foreach (QTcpSocket* socket, clients.keys()) {
        socket->write(msg.toUtf8());
    }
}


void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    qDebug() << "Client connected: " << socket->peerAddress().toString();

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));


    clients.insert(socket, "");
}


void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();

    QString username = clients.value(socket);
    sendToAll(QString("/system:" + username + " has left the chat.\n"));
    clients.remove(socket);
    sendUserList();
}


void Server::onReadyRead() {

    QString servername = "LOCALHOST\\SQLITE";
    QString dbPath="~/db/db"; //맘대로 바꿔서 쓰세연~
    db.setConnectOptions();

    QRegExp signupRex("^/makeID:(.*)/makepw:(.*)/makeemail:(.*)/makegender:([0-1])$");
    QRegExp tokRex("^/email:(.*)/Token:(.*)&");
    QRegExp loginRex("^/userID:(.*)/userPW:(.*)$");
    QRegExp messageRex("^/say:(.*)$");
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        if (loginRex.indexIn(line) != -1) { //login
            QString userID = loginRex.cap(1);
            QString userEnPW = loginRex.cap(2);
            QString userPW = crypto.decryptToString(userEnPW);
            QSqlQuery query;
               query.prepare("SELECT ID,PW FROM info WHERE ID=? AND PW=?");
               query.addBindValue(user);
               query.addBindValue(userPW);
               query.exec();
            //db open
            if(!db.open()){
                qDebug()<<"Error opening DB";
            }
            else qDebug()<<"db open";

            clients[socket] = user;
            sendToAll(QString("/system:" + user + " has joined the chat.\n"));
            sendUserList();
            qDebug() << user << "logged in.";
        }

        else if (messageRex.indexIn(line) != -1) {
            QString user = clients.value(socket);
            QString msg = messageRex.cap(1);
            sendToAll(QString(user + ":" + msg + "\n"));
            qDebug() << "User:" << user;
            qDebug() << "Message:" << msg;
        }

        else if(signupRex.indexIn(line)!=-1){
            QString id=signupRex.cap(1);
            QString enpw=signupRex.cap(2);
            QString dcpw=crypto.decryptToString(enpw);
            QString email=signupRex.cap(3);
            QString gender=signupRex.cap(4);
            QSqlQuery query;
               query.prepare("INSERT INTO info (ID, PW, Email,Enable,Gender) "
                             "VALUES (:ID, :PW, :Email, :Enable, :Gender)");
               query.addBindValue(":ID",id);
               query.addBindValue(":PW",dcpw);
               query.addBindValue(":Email",email);
               query.addBindValue(":Enable",'1');
               query.addBindValue(":Gender",gender);

               query.exec();
            //db open
            if(!db.open()){
                qDebug()<<"Error opening DB";
            }
            else qDebug()<<"db open";

        }

        else {
            qDebug() << "Bad message from " << socket->peerAddress().toString();
        }
    }
}

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
    query.prepare("INSERT INTO info (ID, PW, Email,Enable,Gender) VALUES (:ID, :PW, :Email, :Enable, :Gender);");
    query.addBindValue(":ID",id);
    query.addBindValue(":PW",pw);
    query.addBindValue(":Email",email);
    query.addBindValue(":Enable",'1');
    query.addBindValue(":Gender",gender);

    if(query.exec())
      {
          success = true;
      }
      else
      {
           qDebug() << "addPerson error:"
                    << query.lastError();
      }

      return success;

}


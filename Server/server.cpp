#include "server.h"
#include <QString>
#include <QRegExp>
#include <QtCore/QCoreApplication>
#define Path_to_DB "/home/kim/db/dongdongju.db"

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
    myDB= QSqlDatabase::addDatabase("QSQLITE");
    myDB.setDatabaseName(Path_to_DB);
    QFileInfo checkFile(Path_to_DB);

    if(checkFile.isFile()){
        if(myDB.open()){
            qDebug()<<"[+]DB is open";
        }

    }
    else{
        qDebug()<<"[!]DB error: file not exist";
    }
}


Server::~Server(){
    myDB.close();
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
    qDebug()<<"dshkds";
    QRegExp signupRex("^/makeID:(.*)/makepw:(.*)/makeemail:(.*)/makegender:([0-1])$");
    QRegExp tokRex("^/email:(.*)/Token:(.*)$");
    QRegExp loginRex("^/userID:(.*)/userPW:(.*)$");
    QRegExp messageRex("^/say:(.*)$");
    QSqlQuery query;
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug() << line<<endl;
        if (loginRex.indexIn(line) != -1) { //login
            QString userID = loginRex.cap(1);
            qDebug() << userID<<endl;
            QString userEnPW = loginRex.cap(2);
            qDebug() << userEnPW <<endl;
            QString userPW = crypto.decryptToString(userEnPW);
            qDebug() << userPW<<endl;
            qDebug()<<userID+userPW;

            if(query.exec("SELECT ID,PW FROM info WHERE ID=\'"+userID+"\'AND PW=\'"+userPW+"\'")){
                if(query.next()){
                clients[socket] = userID;
                sendToAll(QString("/system:" + userID + " has joined the chat.\n"));
                sendUserList();
                qDebug() << userID << "logged in.";
                }

                else{
                    qDebug()<<"login fail";
                    return; }     //로그인 승인불가!
            }



        }


        else if (messageRex.indexIn(line) != -1) {//메세지 보내기
            QString user = clients.value(socket);
            QString msg = messageRex.cap(1);
            sendToAll(QString(user + ":" + msg + "\n"));
            qDebug() << "User:" << user;
            qDebug() << "Message:" << msg;
        }

        else if(signupRex.indexIn(line)!=-1){ //회원가입
            QString id=signupRex.cap(1);
            QString enpw=signupRex.cap(2);
            QString dcpw=crypto.decryptToString(enpw);
            QString email=signupRex.cap(3);
            QString gender=signupRex.cap(4);
            QString Token=signupRex.cap(5);
            if(query.exec("INSERT INTO info VALUES(\'"+id+"\',\'"+dcpw+"\',\'"+email+"\',\'"+gender+"\',1")){
                 qDebug() << id << "signed up!.";
            }
            else
                 qDebug() << id << "error,cannot sign up";
        }

        else if(tokRex.indexIn(line)!=-1){
            QString userEmail=tokRex.cap(1); //이메일인증 버튼을 누른 클라이언트의 이메일주소
            QString Token=tokRex.cap(2); // 클라이언트의 이메일로 전송할 토큰값.
            //클라이언트 이메일로 토큰 전송하는 과정 시작.


        }
    }


}



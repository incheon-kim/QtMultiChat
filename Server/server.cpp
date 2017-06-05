#include "server.h"
#include <QString>
#include <QRegExp>
#include <QSqlRecord>
#define Path_to_DB "/home/menukim/git/QtMultiChat/db/ddj.db"  //db path
Server::Server(QObject* parent) : QObject(parent) {
    this->crypto.setKey(0x0c2ad4a4acb9f023);
    server = new QTcpServer(this);
     manager=new RoomManager();
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
void Server::sendUserList()
{
    //????dont' know how to handle values
        //QString line = "/users:" + clients.values().join(',') + "\n";
    //sendToAll(line);
}


void Server::sendToAll(const QString& msg) {
    foreach (QTcpSocket* socket, clients.keys()) {
        socket->write(msg.toUtf8());
    }
}


void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

}


void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();


    int compareRoomNumber=1;
         QVector<Room>::iterator iter;
         userInfo temp;
    temp=clients[socket];

    if(!manager->isEmpty())
         {

             for(iter=manager->beginIterator();iter!=manager->endItertor();++iter)
             {
                 if(compareRoomNumber==temp.roomNumber)
                 {
                     (*iter).out();
                     qDebug()<<"roomPeople"<<(*iter).getPeople();
                     qDebug()<<"roomNumber"<<temp.roomNumber;
                     break;
                 }
                 compareRoomNumber++;
    }


    QString username = clients.value(socket).userName;
    sendToAll(QString("/system:" + username + " has left the chat.\n"));
    clients.remove(socket);
    sendUserList();
}
}

//  /makegender:([0-1])
void Server::onReadyRead() {

    QRegExp signupRex("^/makeID:(.*)/makepw:(.*)/makeemail:(.*)$");
    QRegExp tokRex("^/email:(.*)/Token:(.*)$");
    QRegExp loginRex("^/userID:(.*)/userPW:(.*)$");

    QRegExp messageRex("^(.*):(.*):/say:(.*)$");

    QSqlQuery q(myDB);
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        if (loginRex.indexIn(line) != -1) {

            QString userID = loginRex.cap(1);
            QString userEnPW = loginRex.cap(2);
            QString userPW = crypto.decryptToString(userEnPW);
            QString query="SELECT ID,PW FROM usrInfo WHERE ID=\'"+userID+"\'AND PW=\'"+userPW+"\'";
            QString query2="SELECT Gender FROM usrInfo";
            if(q.exec(query)){
                if(q.next()){
                    q.exec(query2);
                    if(q.next()) {
                        QSqlRecord record = q.record();
                        qDebug() << "gender : " << record.value(1).toInt();
                    }
                userInfo temp;
                temp=clients[socket];
                temp.userName=userID;
                clients[socket] = temp;
                sendToAll(QString("/system:" + userID + " has joined the chat.\n"));
                sendUserList();
                qDebug() << userID << "logged in.";

                sendToAll(QString("/LoginSuccess:"+userID+"\n"));
                QString number;
                int roomNumber=1;
                bool enter=false;
                QVector<Room>::iterator iter;

                   if(!manager->isEmpty())
                   {

                       for(iter=manager->beginIterator();iter!=manager->endItertor();++iter)
                       {
                           if((*iter).getPeople()<2) //need to modify here later(male/female)
                           {
                               (*iter).enter();
                               number="number";
                               enter=true;
                               qDebug()<<"roomPeople"<<(*iter).getPeople();
                               qDebug()<<"roomNumber"<<roomNumber;
                               break;
                           }
                           roomNumber++;
                       }
                   }

                   if(enter==false)
                   {

                       manager->createRoom();
                       QVector<Room>::iterator getRoom= manager->lastIter();
                       (*getRoom).enter();
                       qDebug()<<"roomPeople"<<(*getRoom).getPeople();
                       qDebug()<<"enterRoom"<<roomNumber;
               }

                userInfo info;
                info.roomNumber=roomNumber;
                clients.insert(socket,info);

                QString setClientRoomNumber = QString::number(roomNumber); //tmp is client's room_no
                sendToAll(QString(number + ":" + setClientRoomNumber + "\n")); //send to connected client

                qDebug()<<"sendingtest";
                }

                else{
                    qDebug()<<"login fail";
                    socket->disconnectFromHost();
                }     //로그인 승인불가!
            }

        }

        if (messageRex.indexIn(line) != -1) {
            QString num = messageRex.cap(1);
           QString user = messageRex.cap(2);
            QString msg = messageRex.cap(3);
            qDebug() << "넘버링 테스트: "<<num;
            sendToAll(QString( num + ":"+ user +":"+ msg + "\n"));
            qDebug() << "User:" << user<< "Message:" << msg;
        }


        if(signupRex.indexIn(line)!=-1){ //회원가입
            qDebug()<<"enterd signupRex";
            QString id=signupRex.cap(1);
            QString enpw=signupRex.cap(2);
            QString dcpw=crypto.decryptToString(enpw);
            QString email=signupRex.cap(3);
            int gender =0; //change! test use
            QString gen = QString::number(gender);
            QString Token=signupRex.cap(5);
            QString query="INSERT INTO usrInfo (ID,PW,Gender,Enable) VALUES('"+id+"','"+dcpw+"',"+gen+",1)";
            QString query2="INSERT INTO usrE (ID,Email) VALUES('"+id+"','"+email+"')";
            q.prepare(query);
            qDebug()<<"signupform data came to server";
            if(q.exec(query)==true){
                qDebug()<<"q1 ok";
                    q.prepare(query2);
                    if(q.exec(query2)==true){

                             qDebug() << id << "signed up!.";
                  }
                    else{
                        qDebug()<<q.lastError();
                    }
                }
            else{
                qDebug()<<q.lastError();
            }
            }


        if(tokRex.indexIn(line)!=-1){
            QString userEmail=tokRex.cap(1); //이메일인증 버튼을 누른 클라이언트의 이메일주소
            QString Token=tokRex.cap(2); // 클라이언트의 이메일로 전송할 토큰값.
            //클라이언트 이메일로 토큰 전송하는 과정 시작.
            qDebug()<<Token<<endl<<userEmail;
            sendMail(Token, userEmail);
        }


                }
}
void Server::sendMail(QString Token, QString Destination){
    // 정보
    QString admin = "masterofddj@gmail.com";
    QString pwd = "";
    QString host = "smtp.gmail.com";
    QString Title = "동동주 인증메일 입니다.";
    QString body = "안녕하세요, 동동주 입니다.\n인증번호는 : ";
    body = body +Token+" 입니다. 감사합니다.";

    Smtp* smtp = new Smtp(admin.toUtf8(),pwd.toUtf8(),host.toUtf8(),465);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    smtp->sendMail(admin, Destination, Title.toUtf8(), body.toUtf8());
}

void Server::mailSent(QString status){
    if(status == "Message sent")
        qDebug() <<"Email Successfully sent.";
}

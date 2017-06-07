#include "server.h"
#include <QString>
#include <QRegExp>
#include <QSqlRecord>
#define Path_to_DB "../db/ddj.db"  //db path

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
    sendToAll(QString(QString::number(temp.roomNumber)+":"+"/system:"+"상대방이 나갔습니다.\n"));
    clients.remove(socket);
    sendUserList();
}
}

//  /makegender:([0-1])
void Server::onReadyRead() {
    QRegExp signupRex("^/makeID:(.*)/makepw:(.*)/makeemail:(.*)/makegender:([0-1])$");
    //QRegExp signupRex("^/makeID:(.*)/makepw:(.*)/makeemail:(.*)/makegender:([0-1])$");
    QRegExp tokRex("^/email:(.*)/Token:(.*)$");
    QRegExp loginRex("^/userID:(.*)/userPW:(.*)$");
    QRegExp messageRex("^(.*):(.*):/say:(.*)$");

    QSqlQuery q(myDB);
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug()<<line;
        if(signupRex.indexIn(line) != -1){ //회원가입
            qDebug()<<"enterd signupRex";
            QString id=signupRex.cap(1);
            QString enpw=signupRex.cap(2);
            QString dcpw=crypto.decryptToString(enpw);
            QString email=signupRex.cap(3);
            QString gen = signupRex.cap(4);
            QString enable ="1";
            QString query="INSERT INTO usrInfo (ID,PW,Gender,Enable) VALUES(\'"+id+"\'\,\'"+dcpw+"\'\,"+gen+"\,"+enable+")";
            qDebug()<<query;
            QString query2="INSERT INTO usrE (ID,Email) VALUES('"+id+"','"+email+"')";
            q.prepare(query);
            qDebug()<<"signupform data came to server";
            if(q.exec(query)){
                qDebug()<<"q1 ok";
                    q.prepare(query2);
                    if(q.exec(query2)){

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

        if (loginRex.indexIn(line) != -1) {

            QString userID = loginRex.cap(1);
            QString userEnPW = loginRex.cap(2);
            QString userPW = crypto.decryptToString(userEnPW);
            QString query="SELECT ID,PW FROM usrInfo WHERE ID=\'"+userID+"\'AND PW=\'"+userPW+"\'";
            QString query2="SELECT Gender FROM usrInfo WHERE ID=\'"+userID+"\'";
             qDebug()<<"q sc"<<endl;
             int gender_temp;
            if(q.exec(query)){
                if(q.next()){
                    qDebug()<<"q1 sc"<<endl;
                    q.exec(query2);
                    if(q.next()) {
                         qDebug()<<"q2 sc"<<endl;
                        QSqlRecord record = q.record();
                        int gender =q.record().indexOf("Gender");
                        int gender_temp = q.value(gender).toInt();
                        qDebug() << "gender : " << gender_temp;
                        userInfo temp;
                        temp=clients[socket];
                        temp.userName=userID;
                        temp.userSex=gender_temp;
                        clients[socket]=temp;
                    }
                    /*
                userInfo temp;
                temp=clients[socket];
                temp.userName=userID;
                qDebug() << "gender : " << gender_temp;
                temp.userSex=gender_temp;
                clients[socket] = temp;

                qDebug()<<"ciient's sex"<<clients[socket].userSex;
*/
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
                               qDebug()<<"usersex"<<clients[socket].userSex;
                               if(clients[socket].userSex==0 && (*iter).needMale())
                               {
                                   (*iter).enter();
                                   (*iter).enterMale();
                                   number="number";
                                   enter = true;
                                   qDebug()<<"Enter Male";
                                   qDebug()<<"roomPeople"<<(*iter).getPeople();
                                   qDebug()<<"roomNumber"<<roomNumber;
                                   break;

                               }
                               else if(clients[socket].userSex==1&&(*iter).needFemale())
                               {

                                   (*iter).enter();
                                   (*iter).enterFemale();
                                   number="number";
                                   enter = true;
                                   qDebug()<<"Enter Female";
                                   qDebug()<<"roomPeople"<<(*iter).getPeople();
                                   qDebug()<<"roomNumber"<<roomNumber;
                                   break;
                             }
                           }
                           roomNumber++;
                       }
                   }

                   if(enter==false)
                   {

                         qDebug()<<"usersex"<<clients[socket].userSex;
                       manager->createRoom();
                       QVector<Room>::iterator getRoom= manager->lastIter();
                       if(clients[socket].userSex==0)
                       {
                           (*getRoom).enter();
                           (*getRoom).enterMale();
                           qDebug()<<"ENter Male";
                           qDebug()<<"roomPeople"<<(*getRoom).getPeople();
                           qDebug()<<"enter Room"<<roomNumber;

                       }
                       else{

                           (*getRoom).enter();
                           (*getRoom).enterFemale();
                           qDebug()<<"ENter Female";
                           qDebug()<<"roomPeople"<<(*getRoom).getPeople();
                           qDebug()<<"enter Room"<<roomNumber;

                       }
               }

                userInfo info;
                info.roomNumber=roomNumber;
                clients.insert(socket,info);

                QString setClientRoomNumber = QString::number(roomNumber); //tmp is client's room_no
                sendToAll(QString(number + ":" + setClientRoomNumber + "\n")); //send to connected client

                qDebug()<<"sendingtest";
                // 상대방 접속 메세지
                sendToAll(QString((QString::number(clients[socket].roomNumber))+":"+QString::number(clients[socket].roomNumber)+":"+"/system:"+"상대방이 접속했습니다."));
                }

                else{
                    qDebug()<<"login fail";
                    socket->disconnectFromHost();
                }     //로그인 승인불가!
            }

        }

        else if (messageRex.indexIn(line) != -1) {
            QString num = messageRex.cap(1);
           QString user = messageRex.cap(2);
            QString msg = messageRex.cap(3);
            qDebug() << "넘버링 테스트: "<<num;
            sendToAll(QString( num + ":"+ user +":"+ msg + "\n"));
            qDebug() << "User:" << user<< "Message:" << msg;
        }





        else if(tokRex.indexIn(line)!=-1){
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
    QString pwd = "qwerty1995";
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

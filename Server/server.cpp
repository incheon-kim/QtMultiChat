#include "server.h"

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
    if(!manager->isEmpty()){            
        for(iter=manager->beginIterator();iter!=manager->endItertor();++iter)
        {
            if(compareRoomNumber==temp.roomNumber)
            {
                if(temp.userSex==0){
                    (*iter).out();
                    (*iter).outMale();
                    qDebug()<<"Male left the room";
                    qDebug()<<"roomPeople"<<(*iter).getPeople();
                    qDebug()<<"roomNumber"<<temp.roomNumber;
                    break;
                }
                else
                {
                    (*iter).out();
                    (*iter).outFemale();
                    qDebug()<<"Female left the room";
                    qDebug()<<"roomPeople"<<(*iter).getPeople();
                    qDebug()<<"roomNumber"<<temp.roomNumber;
                    break;
                 }
          }
            compareRoomNumber++;
    }
    QString username = clients.value(socket).userName;
    sendToAll(QString(QString::number(0)+":"+QString::number(temp.roomNumber)+":"+"/system:"+username+":"+"님이 나갔습니다.\n"));
    clients.remove(socket);
    }
}

void Server::onReadyRead() {
    QRegExp signupRex("^/makeID:(.*)/makepw:(.*)/makeemail:(.*)/makegender:([0-1])$");
    QRegExp tokRex("^/email:(.*)/Token:(.*)$");
    QRegExp loginRex("^/userID:(.*)/userPW:(.*)$");
    QRegExp messageRex("^(.*):(.*):/say:(.*)$");
    QSqlQuery q(myDB);
    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug()<< endl <<"string from client" << line<<endl;
        if(signupRex.indexIn(line) != -1){ //회원가입
            qDebug()<<"--Signup--";
            QString id=signupRex.cap(1);
            QString enpw=signupRex.cap(2);
            QString email=signupRex.cap(3);
            QString gen = signupRex.cap(4);
            QString enable ="1";
            QString query="INSERT INTO usrInfo (ID,PW,Gender,Enable) VALUES(\'"+id+"\'\,\'"+enpw+"\'\,"+gen+"\,"+enable+")";
            qDebug()<<query;
            QString query2="INSERT INTO usrE (ID,Email) VALUES('"+id+"','"+email+"')";
            q.prepare(query);
            if(q.exec(query)){
                qDebug()<<"User information insert Query executed";
                    q.prepare(query2);
                    if(q.exec(query2)){

                             qDebug() << id << "Sign up procedure completed"<<endl;
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
            QString enpw;
            QString query="SELECT PW FROM usrInfo WHERE ID=\'"+userID+"\'";
            QString query2="SELECT Gender FROM usrInfo WHERE ID=\'"+userID+"\'";
            if(q.exec(query)){
                if(q.next()){
                    int dc=q.record().indexOf("PW");
                    QString dcp = q.value(dc).toString();
                    enpw=crypto.decryptToString(dcp);
                    if(enpw!=userPW){
                        qDebug()<<"login fail";
                        socket->disconnectFromHost();
                    }
                    qDebug()<<"Login Query 1 completed"<<endl;
                    q.exec(query2);
                    if(q.next()) {
                        qDebug()<<"Login Query 2 completed"<<endl;
                        int gender =q.record().indexOf("Gender");
                        int gender_temp = q.value(gender).toInt();
                        userInfo temp;
                        temp=clients[socket];
                        temp.userName=userID;
                        temp.userSex=gender_temp;
                        clients.insert(socket,temp);
                    }
                qDebug() << userID << "logged in.";

                sendToAll(QString("/LoginSuccess:"+userID+"\n"));
                QString number;
                int roomNumber=1;
                int roomPeople=0;
                bool enter=false;



                QVector<Room>::iterator iter;

                   if(!manager->isEmpty())
                   {       

                       for(iter=manager->beginIterator();iter!=manager->endItertor();++iter)
                       {
                           if((*iter).getPeople()<2)
                           {
                               qDebug()<<"userGender"<<clients[socket].userSex;
                               if(clients[socket].userSex==0 && (*iter).needMale())
                               {
                                   (*iter).enter();
                                   (*iter).enterMale();
                                   number="number";
                                   enter = true;
                                   qDebug()<<"Enter Male";
                                   qDebug()<<"roomPeople"<<(*iter).getPeople();
                                   roomPeople = (*iter).getPeople();
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
                                   roomPeople = (*iter).getPeople();
                                   qDebug()<<"roomNumber"<<roomNumber;
                                   break;                               
                               }
                           }
                           roomNumber++;
                       }
                   }

                   if(enter==false)
                   {

                       qDebug()<<"userGender"<<clients[socket].userSex;
                       manager->createRoom();
                       QVector<Room>::iterator getRoom= manager->lastIter();
                       if(clients[socket].userSex==0)
                       {
                           (*getRoom).enter();
                           (*getRoom).enterMale();
                           qDebug()<<"Enter Male";
                           qDebug()<<"roomPeople"<<(*getRoom).getPeople();
                           roomPeople = (*getRoom).getPeople();
                           qDebug()<<"enter Room"<<roomNumber;

                       }
                       else{

                           (*getRoom).enter();
                           (*getRoom).enterFemale();
                           qDebug()<<"Enter Female";
                           qDebug()<<"roomPeople"<<(*getRoom).getPeople();
                           roomPeople = (*getRoom).getPeople();
                           qDebug()<<"enter Room"<<roomNumber;

                       }
                    }
                userInfo info;
                info=clients[socket];
                info.roomNumber=roomNumber;
                clients.insert(socket,info);

                // send room number info to user
                sendToAll(QString(number + ":" + QString::number(roomNumber) + "\n")); //send to connected client
                if(roomPeople == 1){ // 상대방을 기다리고 있습니다 메시지 전송
                    sendToAll(QString("3:" + QString::number(clients[socket].roomNumber) + ":" + "/system:"+ "" + ":" + "상대방을 기다리고 있습니다.\n"));

                }
                // 상대방 접속 메세지
                    sendToAll(QString((QString::number(roomPeople))+":"+QString::number(clients[socket].roomNumber)+":"+"/system:"+clients[socket].userName+":"+"님이 접속했습니다.\n"));
                    if (roomPeople == 2){ // 채팅을 시작합니다. 메시지 전송
                        sendToAll(QString("3:" + QString::number(clients[socket].roomNumber) + ":" + "/system:"+ "" + ":" + "채팅을 시작합니다.\n"));
                    }
                }

                else{ //로그인 승인불가!
                    qDebug()<<"login fail";
                    socket->disconnectFromHost();
                }
            }
        }

        else if (messageRex.indexIn(line) != -1) {
            QString roomNum = messageRex.cap(1);
            QString userName = messageRex.cap(2);
            QString msg = messageRex.cap(3);
            sendToAll(QString( roomNum + ":"+ userName +":"+ msg + "\n"));
            qDebug() << "From Room number --> " << roomNum;
            qDebug() << "User:" << userName << "Message:" << msg;
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
    // information to access smtp server

    // full address of account
    QString admin = "masterofddj@gmail.com";
    // passwd of account
    QString pwd = "qwerty1995";
    // hostname of gmail server
    QString host = "smtp.gmail.com";
    // title of email
    QString Title = "동동주 인증메일 입니다.";
    // body of email
    QString body = "안녕하세요, 동동주 입니다.\n인증번호는 :  ";
    body = body + Token + "   입니다. 감사합니다.";

    // address, passwrod, hostname, port(gmail uses 465 for TLS)
    Smtp* smtp = new Smtp(admin.toUtf8(),pwd.toUtf8(),host.toUtf8(),465);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    smtp->sendMail(admin, Destination, Title.toUtf8(), body.toUtf8());
    sendToAll(QString(Token+"\n"));
}

void Server::mailSent(QString status){
    if(status == "Message sent"){
        qDebug() <<"Email Successfully sent.";
    }
}

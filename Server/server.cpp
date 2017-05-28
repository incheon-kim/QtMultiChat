#include "server.h"
#include <QString>
#include <QRegExp>


Server::Server(QObject* parent) : QObject(parent) {
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this,   SLOT(onNewConnection()));

    if (!server->listen(QHostAddress::Any, PORT)) {
        qDebug() << "실행 실패...";
    } else {
        qDebug() << "서버가 시작되었습니다.";
    }
}


/* 모든 사용자의 업데이트된 목록을 보내기 */
void Server::sendUserList() {
    //사용자 목록을 전달
    QString line = "/users:" + userInfo.getValues().join(',') + "\n";
    sendToAll(line);
}



/* 모든 사용자에게 메시지 보내기 */
void Server::sendToAll(const QString& msg) {
    foreach (QTcpSocket* socket, userInfo.getUserKeys()) {
        socket->write(msg.toUtf8());
    }
}

/* 서버에 연결될 때 호출되는 슬롯
 * 새로운 클라이언트 */
void Server::onNewConnection() {
    QTcpSocket* socket = server->nextPendingConnection();
    // 로그인되지 않은 상태에서 익명의 클라이언트를 저장
    userInfo.insert_user(socket, ""); //클라이언트 정보를 UserInfo에 저장
    clientQueue.enqueue(userInfo); //UserInfo 클래스를 큐에 저장
    userNumber++; //유저의 수를 하나 늘린다.
    qDebug() << "Client connected: " << socket->peerAddress().toString();
    qDebug() << clientQueue.size() << "명 접속중..."; //현재 몇명이 접속했는지 정보 출력
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
}

/* 클라이언트 연결이 해제 될 때 호출되는 슬롯 */
void Server::onDisconnect() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    qDebug() << "Client disconnected: " << socket->peerAddress().toString();
    UserInfo user;
    QString username = user.getUserName(socket);
    //시스템 메시지
    sendToAll(QString("/system:" + username + " has left the chat.\n"));
    user.removeUser(socket); //유저를 지운다.
    sendUserList();
}

/*클라이언트에서 데이터 수신*/
void Server::onReadyRead() {
    if(checkUserNumber() == 2){ //큐에 대기자가 2명이면
        UserInfo user1 = clientQueue.dequeue(); //큐에서 유저 정보를 빼온다.
        UserInfo user2 = clientQueue.dequeue(); //큐에서 유저 정보를 빼온다.
        QRegExp loginRex("^/login:(.*)$");
        QRegExp messageRex("^/say:(.*)$");
        QTcpSocket* socket = (QTcpSocket*)sender();
        while (socket->canReadLine()) {
            QString line = QString::fromUtf8(socket->readLine()).trimmed();
            /* 메시지 - 사용자 로그 */
            if (loginRex.indexIn(line) != -1) {
                QString user = loginRex.cap(1);
                userInfo.setUserName(socket, user); //유저 이름을 해당 키에 저장
                //시스템 메시지
                sendToAll(QString("/system:" + user + "님께서 입장하셨습니다.\n"));
                sendUserList();
                qDebug() << user << "logged in.";
            }
            /* 채팅 메시지 */
            else if (messageRex.indexIn(line) != -1) {
                QString user = userInfo.getValue(socket);
                QString msg = messageRex.cap(1);
                sendToAll(QString(user + ":" + msg + "\n"));
                qDebug() << "User:" << user;
                qDebug() << "Message:" << msg;
            }
            /* 클라이언트에서 잘못된 메시지 */
            else {
                qDebug() << "Bad message from " << socket->peerAddress().toString();
            }
        }
    }
    else{
        QRegExp loginRex("^/login:(.*)$");
        QRegExp messageRex("^/say:(.*)$");
        QTcpSocket* socket = (QTcpSocket*)sender();
        while (socket->canReadLine()) {
            QString line = QString::fromUtf8(socket->readLine()).trimmed();
            /* 메시지 - 사용자 로그 */
            if (loginRex.indexIn(line) != -1) {
                QString user = loginRex.cap(1);
                userInfo.setUserName(socket, user); //유저 이름을 해당 키에 저장
                sendToAll(QString("/system:" + user + "님께서 입장하셨습니다. \n"));
                sendUserList();
                qDebug() << user << "logged in.";
            }
            /* 채팅 메시지 */
            else if (messageRex.indexIn(line) != -1) {
                QString user = userInfo.getValue(socket);
                QString msg = messageRex.cap(1);
                sendToAll(QString(user + ":" + msg + "\n"));
                qDebug() << "User:" << user;
                qDebug() << "Message:" << msg;
            }
            /* 클라이언트에서 잘못된 메시지 */
            else {
                qDebug() << "Bad message from " << socket->peerAddress().toString();
            }
        }
    }
}

int Server::checkUserNumber(){
   return clientQueue.size();
}

void Server::makeRoom(){
    //유저 2명 정보 리스트에 저장
    //curUser.push_back(clientQueue.dequeue());
    //curUser.push_back(clientQueue.dequeue());
    UserInfo user1;
    UserInfo user2;
    user1 = clientQueue.dequeue();
    userNumber--;
    user2 = clientQueue.dequeue();
    userNumber--;
    onReadyRead();
}




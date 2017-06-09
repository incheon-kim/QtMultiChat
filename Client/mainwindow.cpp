#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTcpSocket>
#include "simplecrypt.h"
#include <QString>


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mainFrame);
    connect(ui->leID, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()));
    connect(ui->lePW, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()));
    connect(ui->leMessage, SIGNAL(returnPressed()), this, SLOT(on_pbSend_clicked()));
    connect(ui->pbSignup,SIGNAL(returnPressed()),this,SLOT(on_pbSignup_clicked()));

    QRegExp regex("^[a-zA-Z]\\w+");
    ui->leID->setValidator(new QRegExpValidator(regex, this));

    user=new User(new QTcpSocket(this));
    //socket = new QTcpSocket(this);
    connect(user->getSocket(), SIGNAL(connected()), this, SLOT(onConnected()));
    connect(user->getSocket(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(user->getSocket(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    user->getSocket()->connectToHost("127.0.0.1",1234);


    // libgtk2.0-dev and libpulse-dev package should be installed
    QMediaPlaylist *bgm = new QMediaPlaylist();
    bgm->addMedia(QUrl("qrc:/sound/bgm.wav"));
    bgm->setPlaybackMode(QMediaPlaylist::Loop);
    QMediaContent noti(QUrl("qrc:/sound/noti.wav"));

    _bgm = new QMediaPlayer;
    _noti = new QMediaPlayer;
    _bgm->setPlaylist(bgm);
    _noti->setMedia(noti);
    _bgm->play();
    //BGM.play("qrc:/sound/noti.wav");
}

MainWindow::~MainWindow() {
    user->getSocket()->close();
    delete ui;
}

void MainWindow::on_pbLogin_clicked() {
    QString userName = ui->leID->text().trimmed();
    if (userName.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "아이디를 입력해 주세요.",
                                 QMessageBox::Ok);
        return;
    }
    user->setUserID(userName);

    QString userPW = ui->lePW->text().trimmed();
    if (userPW.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "비밀번호를 입력해 주세요.",
                                 QMessageBox::Ok);
        return;
    }
    SimpleCrypt crypto;
    crypto.setKey(0x0c2ad4a4acb9f023);
    QString cpw=crypto.encryptToString(userPW);

    user->getSocket()->write(QString("/userID:"+userName+"/userPW:"+cpw+"\n").toUtf8());
}

void MainWindow::on_pbSend_clicked() {
    QString message = ui->leMessage->text().trimmed();
    if (!message.isEmpty()) {
    QString sNumber = QString::number(user->getRoomNumber());
        user->getSocket()->write(QString(sNumber + ":" +user->getUserID()+":"+ "/say:" + message + "\n").toUtf8());
        //user->getSocket()->write(QString("/say:" + message + "\n").toUtf8());
        ui->leMessage->clear();
        ui->leMessage->setFocus();
    }
    _noti->play();
}

void MainWindow::onReadyRead() {
    user->setLoginFlag(true);
    QRegExp numberRex("^(.*):([0-9])$"); //client'snumber
    QRegExp usersRex("^/users:(.*)$");
    QRegExp systemRex("^(.*):(.*):/system:(.*):(.*)$");
    QRegExp messageRex("^(.*):(.*):(.*)$");
    //QRegExp messageRex("^(.*):(.*)$");
    QRegExp loginAcceptRex("^/LoginSuccess:(.*)$");

    while (user->getSocket()->canReadLine()) {
        QString sNumber;
        QString line = QString::fromUtf8(user->getSocket()->readLine()).trimmed();
        if(numberRex.indexIn(line) != -1)
                { //클라이언트에 번호 부여
                    sNumber = numberRex.cap(2);
                    if(user->getRoomNumber() == 0){ //set client's room number;
                        user->setRoomNumber(sNumber.toInt());
                    }
        }
        else if (usersRex.indexIn(line) != -1) {
            QStringList users = usersRex.cap(1).split(",");
            ui->lwUsers->clear();
            foreach (QString user, users) {
                 new QListWidgetItem(QIcon(":/user.png"), user, ui->lwUsers);
            }
        }
        else if (systemRex.indexIn(line) != -1) {
            QString peopleNum = systemRex.cap(1);
            QString roomNum = systemRex.cap(2);
            QString userName = systemRex.cap(3);
            QString msg = systemRex.cap(4);
            if(user->getRoomNumber() == roomNum.toInt() && peopleNum.toInt() != 1){
                if(userName != user->getUserID())
                ui->teChat->append("<p color=\"red\">" + userName +" "+ msg + "</p>\n");
            }
        }
        else if (messageRex.indexIn(line) != -1) {
                    QString curNumber = messageRex.cap(1);
                    if(user->getRoomNumber() == curNumber.toInt()){
                        QString msgUser = messageRex.cap(2);
                        QString message = messageRex.cap(3);
                        ui->teChat->append("<p><b>" + msgUser + "</b>: " + message + "</p>\n");
                        if(user->getUserID() != msgUser)
                        _noti->play();
                    }
        }
        if(loginAcceptRex.indexIn(line)!=-1) {
            QString loginA=loginAcceptRex.cap(1);

            if(!user->getUserID().compare(loginA)){
                 ui->teChat->clear();
                 ui->stackedWidget->setCurrentWidget(ui->chatPage);
                 ui->leMessage->setFocus();
            }
        }
    }

}

void MainWindow::onConnected() {

}


//login fail
void MainWindow::onDisconnected() {
    qDebug() << user->getLoginFlag();
    if(!user->getLoginFlag()){
        QMessageBox::warning(NULL, "알림",
                            "아이디 또는 비밀번호를 확인해 주세요.", QMessageBox::Ok);
    }
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    user->getSocket()->connectToHost("127.0.0.1",1234);
}

void MainWindow::on_pbSignup_clicked (){
    dlsignin form;
    form.setSocket(user->getSocket());
    form.setModal(true);
    form.exec();


}


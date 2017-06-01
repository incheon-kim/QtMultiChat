#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QTcpSocket>
#include "simplecrypt.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mainFrame);
    connect(ui->leID, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()));
    connect(ui->lePW, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()));
    connect(ui->leMessage, SIGNAL(returnPressed()), this, SLOT(on_pbSend_clicked()));
    connect(ui->pbSignup,SIGNAL(returnPressed()),this,SLOT(on_pbSignup_clicked()));

    QRegExp regex("^[a-zA-Z]\\w+");
    ui->leID->setValidator(new QRegExpValidator(regex, this));

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    socket->connectToHost("127.0.0.1",1234);

}

MainWindow::~MainWindow() {
    socket->close();
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

socket->write(QString("/userID:"+userName+"/userPW:"+cpw+"\n").toUtf8());


}

void MainWindow::on_pbSend_clicked() {
    QString message = ui->leMessage->text().trimmed();
    if (!message.isEmpty()) {
        socket->write(QString("/say:" + message + "\n").toUtf8());
        ui->leMessage->clear();
        ui->leMessage->setFocus();
    }
}

void MainWindow::onReadyRead() {
    QRegExp usersRex("^/users:(.*)$");
    QRegExp systemRex("^/system:(.*)$");
    QRegExp messageRex("^(.*):(.*)$");
    while (socket->canReadLine()) {
        QString line = QString::fromUtf8(socket->readLine()).trimmed();

        if (usersRex.indexIn(line) != -1) {
            QStringList users = usersRex.cap(1).split(",");
            ui->lwUsers->clear();
            foreach (QString user, users) {
                 new QListWidgetItem(QIcon(":/user.png"), user, ui->lwUsers);
            }

            }


        else if (systemRex.indexIn(line) != -1) {
            QString msg = systemRex.cap(1);
            ui->teChat->append("<p color=\"gray\">" + msg + "</p>\n");
        }

        else if (messageRex.indexIn(line) != -1) {
            QString user = messageRex.cap(1);
            QString message = messageRex.cap(2);
            ui->teChat->append("<p><b>" + user + "</b>: " + message + "</p>\n");
        }
    }
}

void MainWindow::onConnected() {

    /*
    //login
    ui->teChat->clear();

    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    //socket->write(QString("/login:" + ui->leID->text() + "\n").toUtf8());
    ui->leMessage->setFocus();

    *///login fail
}

void MainWindow::onDisconnected() {
    QMessageBox::warning(NULL, "Warning",
                        "check ID or PW", QMessageBox::Ok);
  ui->stackedWidget->setCurrentWidget(ui->loginPage);
  socket->connectToHost("127.0.0.1",1234);
}




void MainWindow::on_pbSignup_clicked()
{

    dlsignin form;
    form.setSocket(socket);
    form.setModal(true);
    form.exec();
}



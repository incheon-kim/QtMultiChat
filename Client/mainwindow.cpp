#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>
#include <QMessageBox>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mainFrame);
    connect(ui->leServer, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()));
    connect(ui->leName, SIGNAL(returnPressed()), this, SLOT(on_pbLogin_clicked()));
    connect(ui->leMessage, SIGNAL(returnPressed()), this, SLOT(on_pbSend_clicked()));

    /* Имя пользователя должно содержать только латинские буквы,
     * цифры и символы подчеркивания и начинаться только с букв */
    QRegExp regex("^[a-zA-Z]\\w+");
    ui->leName->setValidator(new QRegExpValidator(regex, this));

    user=new User(new QTcpSocket(this));
    connect(user->getSocket(), SIGNAL(connected()), this, SLOT(onConnected()));
    connect(user->getSocket(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(user->getSocket(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pbLogin_clicked() {
    /* Проверка ввода адреса */
    QString serverName = "10.0.2.15"; //server's address
    if (serverName.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "Enter the server name or address.",
                                 QMessageBox::Ok);
        return;
    }

    /* Проверка ввода имени пользователя */
    QString userName = ui->leName->text().trimmed();
    if (userName.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "Enter your nickname.",
                                 QMessageBox::Ok);
        return;
    }

    user->getSocket()->connectToHost(serverName, PORT);
}

void MainWindow::on_pbSend_clicked() {
    QString message = ui->leMessage->text().trimmed();
    if (!message.isEmpty()) {
        QString sNumber = QString::number(user->getRoomNumber());
        user->getSocket()->write(QString(sNumber + ":" + "/say:" + message + "\n").toUtf8());
        ui->leMessage->clear();
        ui->leMessage->setFocus();
    }
}

void MainWindow::onReadyRead() {
    QRegExp numberRex("^(.*):([0-9])$"); //client'snumber
    QRegExp usersRex("^/users:(.*)$");
    QRegExp systemRex("^/system:(.*)$");
    QRegExp messageRex("^(.*):(.*):(.*)$");

    while (user->getSocket()->canReadLine()) {

        //Server's sendToAll(QString(number + ":" + tmp + "\n"));
        QString line = QString::fromUtf8(user->getSocket()->readLine()).trimmed();
        QString sNumber;
        if(numberRex.indexIn(line) != -1)
        { //클라이언트에 번호 부여
            sNumber = numberRex.cap(2);
            if(user->getRoomNumber() == 0) //set client's room number;
            {
                user->setRoomNumber(sNumber.toInt());

            }
        }
        else if (usersRex.indexIn(line) != -1) {
            QStringList users = usersRex.cap(2).split(",");
            ui->lwUsers->clear();
            foreach (QString user, users) {
                new QListWidgetItem(QIcon(":/user.png"), user, ui->lwUsers);
            }
        }
        // Системное сообщение
        else if (systemRex.indexIn(line) != -1) {
            QString msg = systemRex.cap(1);
            ui->teChat->append("<p color=\"gray\">" + msg + "</p>\n");
        }
        // Если сообщение - от пользователя
        else if (messageRex.indexIn(line) != -1) {
            QString curNumber = messageRex.cap(1);
            if(user->getRoomNumber() == curNumber.toInt()){
                qDebug() << "숫자 변환 테스트: " <<sNumber.toInt();
                QString user = messageRex.cap(2);
                QString message = messageRex.cap(3);
                ui->teChat->append("<p><b>" + user + "</b>: " + message + "</p>\n");
            }
        }
    }
}

void MainWindow::onConnected() {
    ui->teChat->clear();

    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    user->getSocket()->write(QString("/login:" + ui->leName->text() + "\n").toUtf8());
    ui->leMessage->setFocus();
}

void MainWindow::onDisconnected() {
    QMessageBox::warning(NULL, "Warning",
                         "You have been disconnected from the server", QMessageBox::Ok);
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

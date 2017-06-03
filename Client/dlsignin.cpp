#include "dlsignin.h"
#include "ui_dlsignin.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include "simplecrypt.h"
#include <QString>
#include <QDebug>

dlsignin::dlsignin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlsignin)
{
    qDebug()<<"dlsignin working";
    ui->setupUi(this);
    socket->connectToHost("127.0.0.1",1234);

}

dlsignin::~dlsignin()
{
    socket->close();
    delete ui;
}


void dlsignin::on_pbSignupform_clicked()
{
    QString makeid=ui->leIDsf->text().trimmed();
    if (makeid.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "아이디를 입력해주세요.",
                                 QMessageBox::Ok);
        return;}
    if(makeid.count()>30){ //id sould be under 30 char
        QMessageBox::information(NULL, "Warning",
                                                 "아이디는 30자 이내로 입력해 주세요.",
                                                   QMessageBox::Ok);
                          return;}


    QString makepw=ui->lePWsf->text().trimmed();
    if (makepw.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "Enter your PW.",
                                 QMessageBox::Ok);
        return;}
    if(makepw.count()>30){
        QMessageBox::information(NULL, "Warning",
                                                   "비밀번호는 30자 이내로 입력해 주세요.",
                                                     QMessageBox::Ok);
                            return;}

    QString checkpw=ui->rePWsf->text().trimmed();
    if(0!=(makepw.compare(checkpw))){
        QMessageBox::information(NULL, "Warning",
                                 "비밀번호를 다시 확인해 주세요.",
                                 QMessageBox::Ok);
        return;
    }
    //encryption
    SimpleCrypt crypto;
    crypto.setKey(0x0c2ad4a4acb9f023);
    QString enpw=crypto.encryptToString(makepw);

    QString makeEmail=ui->leEmail->text().trimmed();
    if(makeEmail.isEmpty()){
        QMessageBox::information(NULL, "Warning",
                                 "이메일을 입력해 주세요.",
                                 QMessageBox::Ok);
        return;}

    int ugender=0;

    if(ui->Female->isChecked())
        ugender=1;

    else if(ui->Male->isChecked())
            ugender=0;

    socket->write(QString("/makeID:"+ makeid +"/makepw:"+enpw+"/makeemail:"+makeEmail+"\n").toUtf8());
    //+"/makegender:"+ugender+
    qDebug()<<"data send to server";
}

void dlsignin::on_EmailAuthen_clicked() //email authentication 이메일 인증
{
    QString makeid=ui->leIDsf->text().trimmed();
    if (makeid.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "아이디를 입력해 주세요.",
                                 QMessageBox::Ok);
        return;
}
    if(makeid.count()>30){ //id sould be under 30 char
        QMessageBox::information(NULL, "Warning",
                                                 "아이디는 30자 이내로 입력해 주세요.",
                                                   QMessageBox::Ok);
                          return;}


    QString makepw=ui->lePWsf->text().trimmed();
    if (makepw.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "비밀번호를 입력해 주세요.",
                                 QMessageBox::Ok);
        return;}
    if(makepw.count()>30){
        QMessageBox::information(NULL, "Warning",
                                                   "비밀번호는 30자 이내로 입력해 주세요.",
                                                     QMessageBox::Ok);
                            return;}

    QString checkpw=ui->rePWsf->text().trimmed();
    if(0!=(makepw.compare(checkpw))){
        QMessageBox::information(NULL, "Warning",
                                 "비밀번호를 확인해 주세요.",
                                 QMessageBox::Ok);
        return;
    }
    QString userEmail=ui->leEmail->text().trimmed();
    userEmail+=ui->emailCombo->currentText();

    // make token value from email
    QString userToken;
    for(int i=0; i<userEmail.size();i++){
        if(i % 2 == 0 && userEmail[i] != '@'){
            userToken += userEmail[i];
        }
    }
    socket->write(QString("/email:"+userEmail+"/Token:"+userToken+"\n" ).toUtf8());

}

void dlsignin::setSocket(QTcpSocket *socket){
    this->socket = socket;
}

void dlsignin::onConnected(){
    qDebug()<<"signupform connected!";

}

void dlsignin::onDisconnected(){
    socket->close();
}

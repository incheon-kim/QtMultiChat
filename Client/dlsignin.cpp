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
    ui->setupUi(this);
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
                                 "Enter your ID.",
                                 QMessageBox::Ok);
        return;}
    if(makeid.count()>30){ //id sould be under 30 char
        QMessageBox::information(NULL, "Warning",
                                                 "ID should be under 30 characters.",
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
                                                   "PW should be under 30 characters.",
                                                     QMessageBox::Ok);
                            return;}

    QString checkpw=ui->rePWsf->text().trimmed();
    if(0!=(makepw.compare(checkpw))){
        QMessageBox::information(NULL, "Warning",
                                 "Check your PW.",
                                 QMessageBox::Ok);
    }
    //encryption
    SimpleCrypt crypto;
    crypto.setKey(0x0c2ad4a4acb9f023);
    QString enpw=crypto.encryptToString(makepw);

    QString makeEmail=ui->leEmail->text().trimmed();
    if(makeEmail.isEmpty()){
        QMessageBox::information(NULL, "Warning",
                                 "Enter your Email.",
                                 QMessageBox::Ok);
        return;}

    int ugender=0;

    if(ui->Female->isChecked())
        ugender=1;

    else if(ui->Male->isChecked())
            ugender=0;

    socket->write(QString("/makeID:"+ makeid +"/makepw:"+enpw+"/makeemail:"+makeEmail+"/makegender:"+ugender).toUtf8());
}

void dlsignin::on_EmailAuthen_clicked() //email authentication 이메일 인증
{
    QString makeid=ui->leIDsf->text().trimmed();
    if (makeid.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "Enter your ID.",
                                 QMessageBox::Ok);
        return;}
    if(makeid.count()>30){ //id sould be under 30 char
        QMessageBox::information(NULL, "Warning",
                                                 "ID should be under 30 characters.",
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
                                                   "PW should be under 30 characters.",
                                                     QMessageBox::Ok);
                            return;}

    QString checkpw=ui->rePWsf->text().trimmed();
    if(0!=(makepw.compare(checkpw))){
        QMessageBox::information(NULL, "Warning",
                                 "Check your PW.",
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
    socket->write(QString("/email:"+userEmail+"/Token:"+userToken ).toUtf8());
}

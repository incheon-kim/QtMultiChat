#include "dlsignin.h"
#include "ui_dlsignin.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include "simplecrypt.h"

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

    QString makepw=ui->lePWsf->text().trimmed();
    if (makepw.isEmpty()) {
        QMessageBox::information(NULL, "Warning",
                                 "Enter your PW.",
                                 QMessageBox::Ok);
        return;}
    //encryption
    SimpleCrypt crypto;
    crypto.setKey(0x0c2ad4a4acb9f023);
    QString enpw=crypto.encryptToString(makepw);
    //QString dcpw=crypto.decryptToString(enpw);
    //QMessageBox::information(NULL, "Warning",enpw,QMessageBox::Ok);
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

    socket->write(QString("/makeID:"+ makeid +"/makepw:"+enpw+"/makeemail"+makeEmail+"/makegender"+ugender).toUtf8());
}

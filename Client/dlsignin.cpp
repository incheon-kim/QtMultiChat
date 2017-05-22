#include "dlsignin.h"
#include "ui_dlsignin.h"

#include <QMessageBox>
dlsignin::dlsignin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlsignin)
{
    ui->setupUi(this);
}

dlsignin::~dlsignin()
{
    delete ui;
}

void dlsignin::on_pbSigninform_clicked()
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
}

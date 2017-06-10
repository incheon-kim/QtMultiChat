#ifndef DLSIGNIN_H
#define DLSIGNIN_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QString>

#include <QTcpSocket>
#include <QCryptographicHash>

#include "simplecrypt.h"
#include "mainwindow.h"

namespace Ui {
class dlsignin;
}

class dlsignin : public QDialog
{
    Q_OBJECT

public:
    explicit dlsignin(QWidget *parent = 0);
    void setSocket(QTcpSocket* socket);
    const QString GetRandomString();
    ~dlsignin();
private slots:
    void on_pbSignupform_clicked();
    void onConnected();
    void onDisconnected();
    void on_EmailAuthen_clicked();
private:
    Ui::dlsignin *ui;
    QTcpSocket* socket;
    QString userToken;
};

#endif // DLSIGNIN_H

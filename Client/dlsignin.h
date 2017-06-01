#ifndef DLSIGNIN_H
#define DLSIGNIN_H

#include <QDialog>
#include <QTcpSocket>
namespace Ui {
class dlsignin;
}

class dlsignin : public QDialog
{
    Q_OBJECT

public:
    explicit dlsignin(QWidget *parent = 0);
    ~dlsignin();

private slots:
    void on_pbSignupform_clicked();

    void on_EmailAuthen_clicked();

private:
    Ui::dlsignin *ui;
     QTcpSocket* socket;
};

#endif // DLSIGNIN_H

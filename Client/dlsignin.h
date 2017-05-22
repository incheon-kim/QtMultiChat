#ifndef DLSIGNIN_H
#define DLSIGNIN_H

#include <QDialog>

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
    void on_pbSigninform_clicked();

private:
    Ui::dlsignin *ui;
};

#endif // DLSIGNIN_H

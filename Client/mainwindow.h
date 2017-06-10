#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>

#include <QRegExp>
#include <QRegExpValidator>

#include <QMessageBox>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "simplecrypt.h"
#include "dlsignin.h"
#include "user.h"

#define HOST "127.0.0.1"
#define PORT 1234


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QMediaPlayer* _bgm;
    QMediaPlayer* _noti;
    QMediaPlayer* _login;
    ~MainWindow();
public slots:
    void on_pbLogin_clicked();
    void on_pbSend_clicked();
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void on_pbSignup_clicked();
private slots:

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    User *user;
    int clientRoomNumber=0;
    bool loginSoundFlag = true;
};


#endif // MAINWINDOW_H

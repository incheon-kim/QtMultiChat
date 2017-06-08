#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QRegExpValidator>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "dlsignin.h"
#include "user.h"
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
};


#endif // MAINWINDOW_H

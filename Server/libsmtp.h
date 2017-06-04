/**
  SMTP lib for QMyInteractiveSynch
  Thank to Raivis Strogonovs : http://morf.lv/modules.php?name=aboutme
    for his help on this sode : http://morf.lv/modules.php?name=tutorials&lasit=20

  HOUSSAIS Grégoire (c)2014

**/

#ifndef LIBSMTP_H
#define LIBSMTP_H

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <QVariant>

#include "libsmtp_global.h"

class LIBSMTPSHARED_EXPORT LibSmtp : public QObject
{
    Q_OBJECT

private:
    QString getAttachement();
    QString getMessage();
    // Mail send informations
    QString from;
    QStringList rcpt;
    int rcptCounter;
    QString subject;
    QString body;
    QStringList files;
    QString friendlyFrom;
    QString friendlyRcpt;

    // Boundaries
    QString boundary;

    // Helpers
    QString setRandomString(const int randomStringLength = 12) const;
    QString getCrlf(int times = 1);


    // Smtp response
    QString response;
    // Message for Smtp
    QString message;

    // text tream to Smtp
    QTextStream *t;

    // sockets
    QSslSocket *sslSocket;
    QTcpSocket *tcpSocket;

    // Auth informations
    QString userAuth;
    QString passAuth;

    // Text method
    bool method;

    // Smtp host
    QString host;

    int timeout;
    int port;
    int state;
    bool ssl;
    bool auth;

    enum states{Tls, HandShake ,Auth,User,Pass,Rcpt,Mail,Data,Init,Body,Quit,Close};

public:
    LibSmtp( const QString &host, int port = 465,
             const QString &user="", const QString &pass="", int timeout = 30000, bool sslTls = false );
    ~LibSmtp();

    enum errors { AuthError };
    enum textMethod { Plain, Html };

//    void sendMail( const QString &from, const QString &to,
//                   const QString &subject, const QString &body, QStringList files,
//                   QString friendlyFrom = QString(""), QString friendlyTo = QString(""));
    void sendMail();

    // -- Getters --

    QString getFrom();
    QStringList getRcpt();
    QString getSubject();
    QString getBody();
    int getPort();
    QString getHost();
    QStringList getFiles();
    QString getFriendlyFrom();
    QString getFriendlyRcpt();
    QString getBoundaries();
    QString getTimeout();
    bool getSslTls();

    bool getAuth();
    QString getUserAuth();
    QString getPassAuth();
    int getTextMethod();

    // -- Setters --
    void setFrom(QString from);
    void setRcpt(QString rcpt);
    void setSubject(QString subject);
    void setBody(QString body);
    void setPort(int port = 25);
    void setHost(QString host="Localhost");
    void setFiles(QStringList files);
    void setFriendlyFrom(QString friendlyFrom);
    void setFriendlyRcpt(QString friendlyRcpt);

    void setBoundaries(QString boundary, bool ended = false);
    void setTimeout(int timeout=30000);
    void setSslTls(bool sslTls=false);

    void setAuth(bool auth=false);
    void setUserAuth(QString userAuth);
    void setPassAuth(QString passAuth);
    void setTextMethod(textMethod method);

signals:
    void status(  const QString & );
    int error(LibSmtp::errors);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();

};

#endif // LIBSMTP_H

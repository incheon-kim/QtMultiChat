#include "libsmtp.h"
#define CRLF "\n";

LibSmtp::LibSmtp(const QString &host, int port, const QString &user, const QString &pass, int timeout, bool sslTls) {
    this->setUserAuth(user);
    this->setPassAuth(pass);
    this->setHost(host);
    this->setPort(port);
    this->setTimeout(timeout);
    this->setSslTls(sslTls);

    if(this->getSslTls()) {
        sslSocket = new QSslSocket(this);
        connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(sslSocket, SIGNAL(connected()), this, SLOT(connected() ) );
        connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(errorReceived(QAbstractSocket::SocketError)));
        connect(sslSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(sslSocket, SIGNAL(disconnected()), this,SLOT(disconnected()));
     } else {
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
        connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected() ) );
        connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(errorReceived(QAbstractSocket::SocketError)));
        connect(tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(tcpSocket, SIGNAL(disconnected()), this,SLOT(disconnected()));
    }

    // set/reset rcpt counter
    rcptCounter = 0;
}
LibSmtp::~LibSmtp()
{
    delete t;
    if(this->getSslTls())
        delete sslSocket;
    else
        delete tcpSocket;
}

void LibSmtp::sendMail()
{
    QString randString = this->setRandomString();

    if(this->getAuth() && (this->getUserAuth().isEmpty() || this->getPassAuth().isEmpty()))
    {
        emit error(LibSmtp::AuthError);
        return;
    }

    message = "";

    // -- To
    message.append("To:");
    if(!this->getFriendlyRcpt().isEmpty())
    {
        message.append(this->getFriendlyRcpt());
    }
    foreach(QString sRcpt, this->getRcpt()) {
        message.append("<" + sRcpt + ">");
    }
//    message.append("<" + this->getRcpt() + ">");
    message.append(this->getCrlf());
    // -- From
    message.append("From:");
    if(!this->getFriendlyFrom().isEmpty())
    {
        message.append(this->getFriendlyFrom());
    }
    message.append("<" + this->getFrom() + ">");
    message.append(this->getCrlf());
    // -- Subject
    message.append("Subject:");
    message.append(this->getSubject());

    message.append(this->getCrlf());
    // -- Let's intitiate multipart MIME with cutting boundary "boundary"
    message.append("MIME-Version: 1.0");
    message.append(this->getCrlf());

    if(this->getTextMethod() == LibSmtp::Html)
    {
        message.append("Content-Type: multipart/alternative;");
    }
    else
    {
        message.append("Content-Type: multipart/mixed;");
    }
    this->setBoundaries(randString);
    message.append("boundary=\"" + randString + "\"");
    message.append(this->getCrlf(2));
    message.append(this->getBoundaries());

    // -- body Plain/Text
    message.append("Content-Type:text/plain; ");
    message.append("charset=UTF-8");
    message.append(this->getCrlf());
    message.append("Content-Transfer-Encoding: quoted-printable");
    message.append(this->getCrlf(2));

    message.append(this->getBody().remove(QRegExp("<[^>]*>")));
    message.append(this->getCrlf(2));
    message.append(this->getBoundaries());


    // -- body Plain/Html
    if(this->getTextMethod() == LibSmtp::Html)
    {
        message.append("Content-Type:text/html; ");
        message.append("charset=UTF-8");
        message.append(this->getCrlf());
        message.append("Content-Transfer-Encoding: quoted-printable");
        message.append(this->getCrlf(2));
        // -- body HTML
        message.append("<html>");
        message.append(this->getCrlf());
            message.append("<head>");
                message.append(this->getCrlf());
            message.append("</head>");
            message.append(this->getCrlf());
            message.append("<body>");
                message.append(this->getCrlf());
                message.append(this->getBody().replace("\n","<br>"));
                message.append(this->getCrlf());
            message.append("</body>");
            message.append(this->getCrlf());
        message.append("</html>");
        message.append(this->getCrlf(2));
    }

    if(!this->files.empty())
    {
        message.append(this->getAttachement());
    }

    // -- end boundary
    this->setBoundaries(randString, true);
    message.append(this->getBoundaries());
    message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
    message.replace( QString::fromLatin1( "\r\n.\r\n" ), QString::fromLatin1( "\r\n..\r\n" ) );

    // -- let's start
    state = Init;

    if(this->getSslTls()) {
        sslSocket->connectToHostEncrypted(host, port); //"smtp.gmail.com" and 465 for gmail TLS
        if (!sslSocket->waitForConnected(timeout)) {
            qDebug() << sslSocket->errorString();
        }
        t = new QTextStream( sslSocket );
    } else {
        tcpSocket->connectToHost(host, port);
        if (!tcpSocket->waitForConnected(timeout)) {
             qDebug() << tcpSocket->errorString();

         }
        t = new QTextStream( tcpSocket );
    }
}

void LibSmtp::stateChanged(QAbstractSocket::SocketState socketState)
{

    qDebug() <<"stateChanged " << socketState;
}

void LibSmtp::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "error " <<socketError;
}

void LibSmtp::disconnected()
{

    qDebug() <<"disconneted";
    qDebug() << "error "  << (this->getSslTls() ? sslSocket->errorString() : tcpSocket->errorString());
    qDebug() << this->getMessage();
}

void LibSmtp::connected()
{
    qDebug() << "Connected ";
}


void LibSmtp::readyRead()
{

     qDebug() <<"readyRead" << state;
    // SMTP is line-oriented

    QString responseLine;
    do
    {
        responseLine = this->getSslTls() ? sslSocket->readLine() : tcpSocket->readLine();
        response += responseLine;
    }
    while ( this->getSslTls() ? sslSocket->canReadLine() : tcpSocket->canReadLine() && responseLine[3] != ' ' );

    responseLine.truncate( 3 );

    qDebug() << "Server response code:" <<  responseLine;
    qDebug() << "Server response: " << response;

    if ( state == Init && responseLine == "220" )
    {
        // banner was okay, let's go on
        *t << "EHLO localhost" <<"\r\n";
        t->flush();

        if(this->getAuth())
        {
            if(this->getSslTls()) {
                state = HandShake;
            } else {
                state = Auth;
            }
        } else {
            state = Mail;
        }
    }
    //No need, because I'm using socket->startClienEncryption() which makes the SSL handshake for you
    /*else if (state == Tls && responseLine == "250")
    {
        // Trying AUTH
        qDebug() << "STarting Tls";
        *t << "STARTTLS" << "\r\n";
        t->flush();
        state = HandShake;
    }*/
    else if (state == HandShake && responseLine == "250")
    {
        sslSocket->startClientEncryption();
        if(!sslSocket->waitForEncrypted(timeout))
        {
            qDebug() << sslSocket->errorString();
            state = Close;
        }


        //Send EHLO once again but now encrypted

        *t << "EHLO localhost" << "\r\n";
        t->flush();
        state = Auth;
    }
    else if (state == Auth && responseLine == "250")
    {
        // Trying AUTH
        qDebug() << "Auth";
        *t << "AUTH LOGIN" << "\r\n";
        t->flush();
        state = User;
    }
    else if (state == User && responseLine == "334")
    {
        //Trying User
        qDebug() << "Username";
        //GMAIL is using XOAUTH2 protocol, which basically means that password and username has to be sent in base64 coding
        //https://developers.google.com/gmail/xoauth2_protocol
        *t << QByteArray().append(userAuth).toBase64()  << "\r\n";
        t->flush();

        state = Pass;
    }
    else if (state == Pass && responseLine == "334")
    {
        //Trying pass
        qDebug() << "Pass";
        *t << QByteArray().append(passAuth).toBase64() << "\r\n";
        t->flush();

        state = Mail;
    }
    else if ( state == Mail && (responseLine == "235" || responseLine == "250") )
    {
        // HELO response was okay (well, it has to be)

        //Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
        *t << "MAIL FROM:<" << from << ">\r\n";
        t->flush();
        state = Rcpt;
    }
    else if ( state == Rcpt && responseLine == "250" )
    {
        //Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
        *t << "RCPT TO:<" << rcpt.at(rcptCounter) << ">\r\n";
        rcptCounter++;
//        *t << "RCPT TO:<" << rcpt << ">\r\n";
        t->flush();

        if(rcptCounter >= rcpt.length())
            state = Data;
        else
            state = Rcpt;
    }
    else if ( state == Data && responseLine == "250" )
    {

        *t << "DATA\r\n";
        t->flush();
        state = Body;
    }
    else if ( state == Body && responseLine == "354" )
    {

        *t << message << "\r\n.\r\n";
        t->flush();
        state = Quit;
    }
    else if ( state == Quit && responseLine == "250" )
    {

        *t << "QUIT\r\n";
        t->flush();
        // here, we just close.
        state = Close;
        emit status( QObject::tr("sent") );
    }
    else if ( state == Close )
    {
        deleteLater();
        return;
    }
    else
    {
        // something broke.
//        QMessageBox::warning( 0, tr( "Qt Simple SMTP client" ), tr( "Unexpected reply from SMTP server:\n\n" ) + response );
        state = Close;
        emit status( QObject::tr("failed") );
    }
    response = "";
}



// -- Getters --
QString LibSmtp::getFrom()
{
    return this->from;
}
QStringList LibSmtp::getRcpt()
{
    return this->rcpt;
}
QString LibSmtp::getSubject()
{
    return this->subject;
}
QString LibSmtp::getBody()
{
    return this->body;
}
int LibSmtp::getPort()
{
    return this->port;
}
QString LibSmtp::getHost()
{
    return this->host;
}
QStringList LibSmtp::getFiles()
{
    return this->files;
}
QString LibSmtp::getFriendlyFrom()
{
    return this->friendlyFrom;
}
QString LibSmtp::getFriendlyRcpt()
{
    return this->friendlyRcpt;
}
QString LibSmtp::getBoundaries()
{
    return this->boundary;
}
bool LibSmtp::getSslTls()
{
    return this->ssl;
}
bool LibSmtp::getAuth()
{
    return this->auth;
}
QString LibSmtp::getUserAuth()
{
    return this->userAuth.simplified().trimmed();
}
QString LibSmtp::getPassAuth()
{
    return this->passAuth.simplified().trimmed();
}
int LibSmtp::getTextMethod()
{
    return this->method;
}

QString LibSmtp::getAttachement()
{
    QStringList files = this->getFiles();
    QString message;
    foreach (QString filePath, files) {
        QFile file(filePath);
        if(file.exists())
        {
            if(!file.open(QIODevice::ReadOnly))
            {
                return QObject::tr( "Couldn't open the file." );
            }
            QByteArray bytes = file.readAll();
            message.append(this->getBoundaries());
            message.append("Content-Type: application/octet-stream" + this->getCrlf());
            message.append("Content-Disposition: attachment; filename="+ QFileInfo(file.fileName()).fileName() + ";" + this->getCrlf());
            message.append("Content-Transfer-Encoding: base64" + this->getCrlf(2) );
            message.append(bytes.toBase64());
            message.append(this->getCrlf());
        }
    }

    return message;
}
QString LibSmtp::getMessage()
{
    return message;
}

// -- Setters --
void LibSmtp::setFrom(QString from)
{
    this->from = from;
}
void LibSmtp::setRcpt(QString rcpt)
{
    this->rcpt << rcpt;
}
void LibSmtp::setSubject(QString subject)
{
    this->subject = subject;
}
void LibSmtp::setBody(QString body)
{
    this->body = body;
}
void LibSmtp::setPort(int port)
{
    this->port = port;
}
void LibSmtp::setHost(QString host)
{
    this->host = host;
}
void LibSmtp::setFiles(QStringList files)
{
    this->files = files;
}
void LibSmtp::setFriendlyFrom(QString friendlyFrom)
{
    this->friendlyFrom = "\"" + friendlyFrom + "\"";
}
void LibSmtp::setFriendlyRcpt(QString friendlyRcpt)
{
    this->friendlyRcpt = "\"" + friendlyRcpt + "\"";
}
void LibSmtp::setBoundaries(QString boundary, bool ended)
{
    this->boundary = "--" + boundary;
    if(ended)
    {
        this->boundary.append("--");
    }
    this->boundary.append(this->getCrlf());
}
void LibSmtp::setTimeout(int timeout)
{
    this->timeout = timeout;
}
void LibSmtp::setSslTls(bool sslTls)
{
    this->ssl = sslTls;
}
void LibSmtp::setAuth(bool auth)
{
    this->auth = auth;
}
void LibSmtp::setUserAuth(QString userAuth)
{
    this->userAuth = userAuth;
}
void LibSmtp::setPassAuth(QString passAuth)
{
    this->passAuth = passAuth;
}
void LibSmtp::setTextMethod(textMethod method)
{
    this->method = method;
}

// -- Helpers --
QString LibSmtp::setRandomString(const int randomStringLength) const
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
//   const int randomStringLength = 12; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}
QString LibSmtp::getCrlf(int times)
{
    QString s_crlf = CRLF;
    s_crlf = s_crlf.repeated(times);
    return s_crlf;
}

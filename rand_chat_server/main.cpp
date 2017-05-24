#include <QCoreApplication>
#include "mainserver.h"
#include "protocol.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainServer mainServer;
    mainServer.Init(8888);
    mainServer.Run();



    return a.exec();
}

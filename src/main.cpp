#include <QCoreApplication>
#include <QDebug>
#include <server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    bool res = server.start(53);
    if(res)
        qDebug() << "OK";
    else
        qDebug() << "Failed.";

    return a.exec();
}

// nslookup -q=a -debug google.com 127.0.0.1

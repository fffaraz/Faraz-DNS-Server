#include <QCoreApplication>
#include <server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    server.start(53);

    return a.exec();
}

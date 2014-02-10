#include <iostream>
#include <QCoreApplication>
#include <dnsserver.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DnsServer server;
    bool r = server.start();

    if(r)
        cout << "OK." << endl;
    else
        cout << "Failed." << endl;

    return a.exec();
}

// nslookup -q=a -debug google.com 127.0.0.1

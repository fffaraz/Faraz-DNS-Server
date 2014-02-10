#ifndef DNSSERVER_H
#define DNSSERVER_H

#include <iostream>
using namespace std;

#include <QHostAddress>
#include <QUdpSocket>

#include "dns.h"

class DnsServer : public QObject
{
    Q_OBJECT

public:
    explicit DnsServer(QObject *parent = 0);
    int start(QHostAddress address = QHostAddress((quint32) 0), quint16 port = 53, bool reuse = false);
    void setIP(QString ip);

private:
    quint32 giveout_ip;
    QUdpSocket udpsocket;
    DNS dns;
    void logRequest(QHostAddress &sender, quint16 senderPort);

private slots:
    void onRequest();
};

#endif // DNSSERVER_H

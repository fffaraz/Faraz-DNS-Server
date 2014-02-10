#ifndef DNSSERVER_H
#define DNSSERVER_H

#include <QObject>
#include <QDebug>
#include <QHostAddress>
#include <QUdpSocket>

#include "dns.h"

class DnsServer : public QObject
{
    Q_OBJECT

public:
    explicit DnsServer(QObject *parent = 0);
    int start(quint16 port = 53, bool reuse = false);


private:
    QUdpSocket udpsocket;
    DNS dns;
    void logRequest(QHostAddress &sender, quint16 senderPort, QByteArray &datagram);
    void sendResponse(QHostAddress &sender, quint16 senderPort, QByteArray &datagram);
    void responseIP(QByteArray &datagram, quint32 ip);

private slots:
    void onRequest();
    
signals:
    
public slots:
    
};

#endif // DNSSERVER_H
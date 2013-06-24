#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QHostAddress>
#include <QUdpSocket>

#include "dns.h"

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = 0);
    int start(quint16 port);

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

#endif // SERVER_H

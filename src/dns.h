#ifndef DNS_H
#define DNS_H

#include <QByteArray>
#include <QDebug>
#include <QtEndian>
#include <QHostAddress>
#include <QString>

#include "dns_packet.h"

class DNS
{
public:
    DNS();
    void process(QByteArray &datagram);
    void addArecordIP(QByteArray &datagram, quint32 ip);
    void showPacket(QByteArray &datagram);
    QString getName(QByteArray &datagram, int index, int *end);
};

#endif // DNS_H

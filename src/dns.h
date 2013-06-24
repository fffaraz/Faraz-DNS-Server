#ifndef DNS_H
#define DNS_H

#include <QByteArray>
#include <QDebug>
#include <QtEndian>
#include "dns_packet.h"

class DNS
{
public:
    DNS();
    void addArecordIP(QByteArray &datagram, quint32 ip);
    void showPacket(QByteArray &datagram);
};

#endif // DNS_H

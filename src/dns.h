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

    QByteArray *datagram;
    int datagramSize;
    char *dataptr;

    bool is_valid;
    DNS_HEADER header;
    QString domain;

    void parse(QByteArray *datagram);
    void makeAnswer();
    void addArecord(quint32 ip, quint32 ttl = 86400);
    void showPacket();

    static QString getName(QByteArray &datagram, int index, int *end);
};

#endif // DNS_H

#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent)
{
    connect(&udpsocket, SIGNAL(readyRead()), this, SLOT(onRequest()));
}

int Server::start(quint16 port)
{
    return udpsocket.bind(port, QUdpSocket::ReuseAddressHint);
}

void Server::onRequest()
{
    while(udpsocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpsocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpsocket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        int datagramSize = datagram.size();
        logRequest(sender, senderPort, datagramSize);
        response(sender, senderPort, datagram, datagramSize);
    }
}

void Server::logRequest(QHostAddress &sender, quint16 senderPort, int datagramSize)
{
    static int counter = 1;
    qDebug() << counter++ << sender.toString() << senderPort << datagramSize;
}

void Server::response(QHostAddress &sender, quint16 senderPort, QByteArray &datagram, int datagramSize)
{
    quint32 ip = QHostAddress("127.0.0.1").toIPv4Address();
    responseIP(sender, senderPort, datagram, datagramSize, ip);
}

void Server::responseIP(QHostAddress &sender, quint16 senderPort, QByteArray &datagram, int datagramSize, quint32 ip)
{
    char *data = datagram.data();
    // We only answer questions
    if(datagramSize < 3 || (data[2] & 0x80) != 0x00) return;

    // Prepare the reply
    if(datagramSize > 12)
    {
        // Make this an answer
        data[2] |= 0x80;
        if(data[11] == 0)
        {
            // EDNS not supported
            // We add an additional answer
            data[7]++;
        }
        else
        {
            data[3] &= 0xf0;
            data[3] |= 4; // NOTIMPL
        }
    }

    const int P_LEN = 16;
    unsigned char packet[] = { 0xc0,0x0c,0x00,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x04,0x7f,0x7f,0x7f,0x7f };
    packet[12] = (ip & 0xff000000) >> 24;
    packet[13] = (ip & 0x00ff0000) >> 16;
    packet[14] = (ip & 0x0000ff00) >>  8;
    packet[15] = (ip & 0x000000ff);

    if(data[11] == 0)
    {
        // Again, EDNS not supported
        datagram.append((char*)packet, P_LEN);
    }

    // Send the reply
    udpsocket.writeDatagram(datagram, sender, senderPort);
}

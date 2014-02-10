#include "dnsserver.h"

DnsServer::DnsServer(QObject *parent) :
    QObject(parent)
{
    giveout_ip = QHostAddress("127.0.0.1").toIPv4Address();
    connect(&udpsocket, SIGNAL(readyRead()), this, SLOT(onRequest()));
}

int DnsServer::start(QHostAddress address, quint16 port, bool reuse)
{
    return udpsocket.bind(address, port, reuse?QUdpSocket::ReuseAddressHint:QUdpSocket::DefaultForPlatform);
}

void DnsServer::setIP(QString ip)
{
    giveout_ip = QHostAddress(ip).toIPv4Address();
}

void DnsServer::onRequest()
{
    while(udpsocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpsocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpsocket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        dns.parse(&datagram);
        dns.makeAnswer();
        dns.addArecord(giveout_ip);

        if(dns.is_valid)
            udpsocket.writeDatagram(datagram, sender, senderPort);

        logRequest(sender, senderPort);
    }
}

void DnsServer::logRequest(QHostAddress &sender, quint16 senderPort)
{
    static int counter = 0;
    QString strlog;
    strlog.append(QString::number(++counter) + ". ");
    strlog.append(sender.toString());
    strlog.append(":" + QString::number(senderPort));
    strlog.append(" (" + QString::number(dns.datagramSize) + ")");

    if(dns.is_valid)
        strlog.append(" >> " + dns.domain);
    else
        strlog.append(" >> INVALID");

    cout << strlog.toStdString() << endl;
    //dns.showPacket();
}



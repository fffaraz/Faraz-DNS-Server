#include "dnsserver.h"

DnsServer::DnsServer(QObject *parent) :
    QObject(parent)
{
    connect(&udpsocket, SIGNAL(readyRead()), this, SLOT(onRequest()));
}

int DnsServer::start(quint16 port, bool reuse)
{
    return udpsocket.bind(port, reuse? QUdpSocket::ReuseAddressHint : QUdpSocket::DefaultForPlatform);
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
        sendResponse(sender, senderPort, datagram);
        logRequest(sender, senderPort, datagram);
    }
}

void DnsServer::logRequest(QHostAddress& sender, quint16 senderPort, QByteArray& datagram)
{
    static int counter = 0;
    QString strlog;
    strlog.append(QString::number(++counter) + ". ");
    strlog.append(sender.toString() + ":" + QString::number(senderPort));
    strlog.append(" -> " + QString::number(datagram.size()));
    qDebug() << strlog.toStdString().c_str();
    dns.showPacket(datagram);
}

void DnsServer::sendResponse(QHostAddress& sender, quint16 senderPort, QByteArray& datagram)
{
    dns.process(datagram);
    udpsocket.writeDatagram(datagram, sender, senderPort);
}

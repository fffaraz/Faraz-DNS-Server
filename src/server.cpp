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
        logRequest(sender, senderPort, datagram);
        sendResponse(sender, senderPort, datagram);
    }
}

void Server::logRequest(QHostAddress &sender, quint16 senderPort, QByteArray &datagram)
{
    static int counter = 0;
    QString strlog;
    strlog.append(QString::number(++counter) + ". ");
    strlog.append(sender.toString() + ":" + QString::number(senderPort));
    strlog.append(" -> " + QString::number(datagram.size()));
    qDebug() << strlog.toStdString().c_str();
    dns.showPacket(datagram);
}

void Server::sendResponse(QHostAddress &sender, quint16 senderPort, QByteArray &datagram)
{
    dns.process(datagram);
    udpsocket.writeDatagram(datagram, sender, senderPort);
}

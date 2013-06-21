#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QHostAddress>
#include <QUdpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    int start(quint16 port);

private:
    QUdpSocket udpsocket;
    void logRequest(QHostAddress &sender, quint16 senderPort, int datagramSize);
    void response(QHostAddress &sender, quint16 senderPort, QByteArray &datagram, int datagramSize);
    void responseIP(QHostAddress &sender, quint16 senderPort, QByteArray &datagram, int datagramSize, quint32 ip);

private slots:
    void onRequest();
    
signals:
    
public slots:
    
};

#endif // SERVER_H

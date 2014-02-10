#include "dns.h"

DNS::DNS()
{
    is_valid = false;
}

void DNS::parse(QByteArray *datagram)
{
    this->datagram = datagram;
    datagramSize = datagram->size();
    dataptr = datagram->data();

    // We only answer questions
    if(datagramSize < 12)
    {
        is_valid = false;
        return;
    }

    if((dataptr[2] & 0x80) != 0x00)
    {
        is_valid = false;
        return;
    }

    is_valid = true;

    // DNS_HEADER header;
    //    QUESTION question;
    //    ANSWER answer;
    //    RES_RECORD res_record;
    //    QUERY query;

    memcpy(&header, dataptr, 12);
    header.id = qFromBigEndian(header.id);
    header.q_count = qFromBigEndian(header.q_count);
    header.ans_count = qFromBigEndian(header.ans_count);
    header.auth_count = qFromBigEndian(header.auth_count);
    header.add_count = qFromBigEndian(header.add_count);

    domain = getName(*datagram, 12, 0);
}

void DNS::makeAnswer()
{
    if(!is_valid) return;

    // recursion available
    // dataptr[3] |= 0x80;

    // Make this an answer
    dataptr[2] |= 0x80;
}

void DNS::addArecord(quint32 ip, quint32 ttl)
{
    if(!is_valid) return;

    // TODO: TTL
    Q_UNUSED(ttl);

    // DNS Answer
    unsigned char QAnswer[] = {
        0xc0,0x0c, // 1100 0000 0000 1100 -> offset = 12
        0x00,0x01, // Type  : A
        0x00,0x01, // Class : IN
        0x00,0x01,0x51,0x80, // TTL = 86400 -> 24h
        0x00,0x04, // RD Length
        0x00,0x00,0x00,0x00 // RDATA
    };

    QAnswer[12] = (ip & 0xff000000) >> 24;
    QAnswer[13] = (ip & 0x00ff0000) >> 16;
    QAnswer[14] = (ip & 0x0000ff00) >>  8;
    QAnswer[15] = (ip & 0x000000ff);

    if(dataptr[11] == 0)
    {
        // We add an additional answer
        dataptr[7]++;
        datagram->append((char*)QAnswer, 16);
    }
    else
    {
        // EDNS not supported
        dataptr[3] &= 0xf0;
        dataptr[3] |= 4; // NOTIMPL
    }
}

void DNS::showPacket()
{
    if(!is_valid) return;

    qDebug() << "----------";
    qDebug() << "ID" << header.id;          // data[0,1]

//  qDebug() << "RD" << header.rd;          // data[2][0]
//  qDebug() << "TC" << header.tc;          // data[2][1]
//  qDebug() << "AA" << header.aa;          // data[2][2]
//  qDebug() << "Opcode" << header.opcode;  // data[2][3,4,5,6]
    qDebug() << "QR" << header.qr;          // data[2][7]

    qDebug() << "rcode" << header.rcode;    // data[3][0,1,2,3]
//  qDebug() << "cd" << header.cd;          // data[3][4]
//  qDebug() << "ad" << header.ad;          // data[3][5]
//  qDebug() << "z" << header.z;            // data[3][6]
//  qDebug() << "RA" << header.ra;          // data[3][7]

    qDebug() << "q_count" << header.q_count;        // data[4,5]
    qDebug() << "ans_count" << header.ans_count;    // data[6,7]
    qDebug() << "auth_count" << header.auth_count;  // data[8,9]
    qDebug() << "add_count" << header.add_count;    // data[10,11]

    qDebug() << "++++++++++";
    qDebug() << domain;
    qDebug() << "----------";
}

QString DNS::getName(QByteArray &datagram, int index, int *end)
{
    QByteArray buffer;
    char *data = datagram.data();
    int   size = datagram.size();
    unsigned char count = data[index++];
    while(count != 0 && (count & 0xc0) != 0xc0 && index+count < size)
    {
        //qDebug() << "COUNT" << count;
        buffer.append(&data[index], (int)count);
        buffer.append('.');
        index += count;
        count = data[index++];
    }
    if(end !=0 ) *end = index;
    return buffer;
}

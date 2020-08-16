#ifndef MAINOBJ_H
#define MAINOBJ_H

#include <QObject>
#include "../protocol/protocol.h"
#include <QQueue>
#include <QtSerialPort>

class mainObj : public QObject
{
    Q_OBJECT
public:
    explicit mainObj(QObject *parent = nullptr);

signals:
    void receiveRX(Receive_Package_16 &);
    void receiveTXStat(Transmit_Status &);

public slots:
    void send(QByteArray &);
    void receiveData();

public:
    QSerialPort *serial;
    QByteArray incomeBuffer;
};

#endif // MAINOBJ_H

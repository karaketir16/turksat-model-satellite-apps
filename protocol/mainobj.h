#ifndef MAINOBJ_H
#define MAINOBJ_H

#include <QObject>
#include "../protocol/protocol.h"
#include <QQueue>
#include <QSerialPort>

class mainObj : public QObject
{
    Q_OBJECT
public:
    explicit mainObj(QObject *parent = nullptr);

signals:
    void receive(QByteArray);
    void receiveTXStat(Transmit_Status);

public slots:
    void send(QByteArray, uint16_t receiver);
    void receiveData();

public:
    QSerialPort *serial;
    QByteArray incomeBuffer;
};

#endif // MAINOBJ_H

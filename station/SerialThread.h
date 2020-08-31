#ifndef _SERIALTHREAD_
#define _SERIALTHREAD_

#include <QThread>
#include <QQueue>
#include "../protocol/protocol.h"
#include <QMutex>
#include <QElapsedTimer>
#include <QtSerialPort>

class SerialThread : public QThread
{
    Q_OBJECT
    void run() override;
signals:
    void resultReady(const QString &s);
    void send(QByteArray &);
public:
    QQueue<Transmit_Request_16> transmitQueues[2];
    QQueue<Receive_Package_16> receiveQueue;

    QElapsedTimer timeoutTimer;
    QByteArray incomeBuffer;

    Transmit_Request_16 checkQueues();

    QQueue<Transmit_Status> tsQueue;
    QQueue<QByteArray> sentQueue;

    QMutex transmitQueuesMutex, receiveQueueMutex, transmitStatusMutex, sentQueueMutex;

//    bool send(QByteArray &);

    QObject dummyPrent;


public slots:
//    void queueChecker();
    void loop();
    void receiveRX(Receive_Package_16 &);
    void receiveTXStatus(Transmit_Status &);
};

#endif

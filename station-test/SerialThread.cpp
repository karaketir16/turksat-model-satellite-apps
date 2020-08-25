#include "SerialThread.h"

#include <QDebug>

uint8_t frameID = 1;

void SerialThread::run(){
    QTimer::singleShot(0,this, &SerialThread::loop);

    this->exec();
}

int resended, success;

void SerialThread::loop(){
    if( ! tsQueue.empty())
    {
        auto ts = tsQueue.dequeue();
        QByteArray tmp(sizeof (ts),0x00);
        memcpy(tmp.data(), &ts, sizeof (ts));
        qDebug() << "Status: " << ts.deliveryStatus << " stat: " << tmp.toHex() << " time: " << timeoutTimer.elapsed();
        if(ts.deliveryStatus == 0x00){
            sentQueueMutex.lock();
            sentQueue.dequeue();
            sentQueueMutex.unlock();
            success++;
            qDebug() << "resended: " << resended << " remain: " << transmitQueues[1].size();

        }
        else{
            emit send(sentQueue.head());
            resended++;
            timeoutTimer.restart();
        }
    }
    if(sentQueue.empty()){
        auto tx = checkQueues();
        if(tx.header.frameType != NO_FRAME){
            auto packageLenght = tx.header.lenght_low + 4;
//            if(frameID == 255) frameID++;
            tx.frameID = frameID;
            QByteArray toSent(packageLenght, 0x00);
            memcpy(toSent.data(), &tx, packageLenght);
            sentQueueMutex.lock();
            sentQueue.enqueue(toSent);
            sentQueueMutex.unlock();
//            qDebug() << "Send it v0.1: " << toSent.toHex();
            emit (send(toSent));
            timeoutTimer.restart();
        }
    }
    QTimer::singleShot(0,this, &SerialThread::loop);
}

void SerialThread::receiveRX(Receive_Package_16 &rx){
    receiveQueueMutex.lock();
    receiveQueue.enqueue(rx);
    receiveQueueMutex.unlock();
}

void SerialThread::receiveTXStatus(Transmit_Status &ts){
    transmitStatusMutex.lock();
    tsQueue.append(ts);
    transmitStatusMutex.unlock();
}


Transmit_Request_16 SerialThread::checkQueues(){
    for(int i =0;i < 2;i++)
    {
        transmitQueuesMutex.lock();
        if( ! transmitQueues[i].isEmpty()){
            transmitQueuesMutex.unlock();
            return transmitQueues[i].dequeue();
        }
        else
        {
            transmitQueuesMutex.unlock();
        }
    }
    Transmit_Request_16 tx;
    return tx;
}

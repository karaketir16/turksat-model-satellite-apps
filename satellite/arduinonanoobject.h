#ifndef ARDUINONANOOBJECT_H
#define ARDUINONANOOBJECT_H

#include <QObject>

extern "C"{
    #include "nano_packets.h"
}



#include <QQueue>
#include <QSerialPort>

class ArduinoNanoObject : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoNanoObject(QObject *parent = nullptr);

signals:
    void receive(nano_package);

public slots:
    void receiveData();

public:
    QSerialPort *serial;
    QByteArray incomeBuffer;

};

#endif // ARDUINONANOOBJECT_H

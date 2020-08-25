QT -= gui
QT += serialport


CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../protocol/abstracttelemetryobject.cpp \
        ../protocol/mainobj.cpp \
        ../protocol/protocol.cpp \
        ../protocol/telemetry_protocol.cpp \
        bme280/bme280.c \
        bme280/bme280_wrapper.c \
        lsm9ds0/SFE_LSM9DS0.cpp \
        main.cpp \
        my_i2c/my_i2c.c \
        nano_packets.c \
        satellitetelemetryobject.cpp \
        test.cpp \
        test2.cpp

# Default rules for deployment.
target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    nano_packets.h \
    ../protocol/abstracttelemetryobject.h \
    ../protocol/mainobj.h \
    ../protocol/protocol.h \
    ../protocol/telemetry_protocol.h \
    bme280/bme280.h \
    bme280/bme280_defs.h \
    bme280/bme280_wrapper.h \
    lsm9ds0/SFE_LSM9DS0.h \
    my_i2c/my_i2c.h \
    satellitetelemetryobject.h

QT -= gui
QT += network sql

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


HEADERS += \
    Data/buffer.h \
    Data/command.h \
    Data/item.h \
    Data/room.h \
    Objects/Clients/clientslist.h \
    Objects/Controlers/controlerlist.h \
    Objects/abstractclient.h \
    Objects/androidclient.h \
    Objects/controler.h \
    Objects/socketslist.h \
    server.h

SOURCES += \
        Data/buffer.cpp \
        Data/command.cpp \
        Data/item.cpp \
        Data/room.cpp \
        Objects/Clients/clientslist.cpp \
        Objects/Controlers/controlerlist.cpp \
        Objects/abstractclient.cpp \
        Objects/androidclient.cpp \
        Objects/controler.cpp \
        Objects/socketslist.cpp \
        main.cpp \
        server.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

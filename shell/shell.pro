
QT += network

#TARGET=shell
SOURCES=main.cpp shell.cpp \
    argumentscollector.cpp
HEADERS=shell.h \
    argumentscollector.h
INCLUDEPATH += include/

LIBS += -L$${PWD}\lib -lQSshd -lQSsh

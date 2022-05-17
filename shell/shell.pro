
QT += network

#TARGET=shell
SOURCES=main.cpp shell.cpp
HEADERS=shell.h
INCLUDEPATH += include/

LIBS += -Llib/ -lQSshd

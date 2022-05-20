QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    login.cpp \
    maincmd.cpp \
    ssh.cpp \
    ssh_argument.cpp

HEADERS += \
    login.h \
    maincmd.h \
    ssh.h \
    ssh_argument.h

FORMS += \
    login.ui \
    maincmd.ui

TRANSLATIONS += \
    easyssh_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += include/

LIBS += -L$${PWD}\lib -lQSshd
#-lQSsh

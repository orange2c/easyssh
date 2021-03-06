QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    login.cpp \
    ssh.cpp \
    pagecmd.cpp \
    ecurosr.cpp \
    etext.cpp \
    eshadow.cpp

HEADERS += \
    login.h \
    ssh.h \
    pagecmd.h \
    ecurosr.h \
    etext.h \
    eshadow.h

FORMS += \
    login.ui \
    pagecmd.ui

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

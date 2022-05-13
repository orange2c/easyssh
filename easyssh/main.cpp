#include "login.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>



#include <netinet/in.h>
#include <sys/select.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <sys/time.h>

#include <sys/types.h>

#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>


extern "C"{
//    #include "libssh2_config.h"
    #include <libssh2.h>
}

int main(int argc, char *argv[])
{



    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "easyssh_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    login w;
    w.show();
    return a.exec();
}

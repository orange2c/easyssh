#include "login.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTextCodec>

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>



int main(int argc, char *argv[])
{

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

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

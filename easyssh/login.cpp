#include "login.h"
#include "ui_login.h"
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <QString>


login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);


//    ssh->set_login_info("127.0.0.1","cb","123456");
    qDebug("connect_ip status:%d", ssh->connect_ip("127.0.0.1" ));
    qDebug("login status:%d", ssh->login("cb","123456" ));
    qDebug( "write:%d",  ssh->write("pwd"));
    qDebug("read:%s", qPrintable( ssh->read() ) );
    qDebug( "write:%d",  ssh->write("pwd"));
    qDebug("read:%s", qPrintable( ssh->read() ) );
}

login::~login()
{
    delete ui;
}


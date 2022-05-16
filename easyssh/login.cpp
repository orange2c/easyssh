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


//    qDebug("connect_ip status:%d", ssh->connect_ip("127.0.0.1" ));
//    qDebug("login status:%d", ssh->login("cb","123456" ));
//    qDebug( "write:%d",  ssh->write("pwd"));
//    qDebug("read:%s", qPrintable( ssh->read() ) );
//    qDebug( "write:%d",  ssh->write("pwd"));
//    qDebug("read:%s", qPrintable( ssh->read() ) );

    ui->Edit_ip->setText("127.0.0.1");
    ui->Edit_user->setText("cb");
    ui->Edit_password->setText("123456");

}

login::~login()
{
    delete ui;
}


void login::on_pushButton_released()
{

    if( ssh->connect_ip(ui->Edit_ip->toPlainText()) != 0 )
    {
        qDebug("ip error");
        return;
    }
    if( ssh->login( ui->Edit_user->toPlainText(), ui->Edit_password->toPlainText() ) !=0 )
    {
        return;
    }

    qDebug("登陆成功");
    MainCmd *mcmd = new MainCmd();
    mcmd->setssh(ssh);
    mcmd->show();
    this->close();

}


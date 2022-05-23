#include "login.h"
#include "ui_login.h"
#include <QString>


login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    ssh = new SSH( );
    ui->Edit_ip->setText( "192.168.103.116" );
    ui->Edit_user->setText( "cb" );
    ui->Edit_password->setText( "123456" );


}

login::~login()
{
    delete ui;
}


void login::on_pushButton_released()
{

//    if( ssh->connect_ip(ui->Edit_ip->toPlainText()) != 0 )
//    {
//        qDebug("ip error");
//        return;
//    }
//    if( ssh->login( ui->Edit_user->toPlainText(), ui->Edit_password->toPlainText() ) !=0 )
//    {
//        return;
//    }
    if( ssh->login( ui->Edit_ip->toPlainText(), ui->Edit_user->toPlainText(), ui->Edit_password->toPlainText() ) < 0 )
    {
        qDebug("登陆失败");
        return;
    }
    qDebug("登陆成功");
//    qDebug( "write:%d",  ssh->write("cd .."));
//    qDebug("read:%s", qPrintable( ssh->read() ) );
//        qDebug( "write:%d",  ssh->write("pwd"));
//        qDebug("read:%s", qPrintable( ssh->read() ) );

    MainCmd *mcmd = new MainCmd();
//    mcmd->setssh(ssh);
    mcmd->show();
    this->close();

}


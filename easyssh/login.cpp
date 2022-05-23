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

    connect( ssh, SIGNAL(connect_success()), this, SLOT(connect_success()) );
    connect( ssh, SIGNAL(connect_error()), this, SLOT(connect_error()) );
}

login::~login()
{
    delete ui;
}

void login::connect_success()
{
    MainCmd *mcmd = new MainCmd(ssh);
    mcmd->show();
    this->close();
}
void login::connect_error()
{
    ui->pushButton->setEnabled(true);
}


void login::on_pushButton_released()
{
    ssh->login( ui->Edit_ip->toPlainText(), ui->Edit_user->toPlainText(), ui->Edit_password->toPlainText() );
    ui->pushButton->setEnabled(false);
}



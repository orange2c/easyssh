#include "login.h"
#include "ui_login.h"
#include <libssh2.h>
#include <libssh2_sftp.h>

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

}

login::~login()
{
    delete ui;
}


#include "maincmd.h"
#include "ui_maincmd.h"

MainCmd::MainCmd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainCmd)
{
    ui->setupUi(this);

    ui->Edit_cmd->insertPlainText("sss");
    ui->Edit_cmd->insertPlainText("hello");

}

MainCmd::~MainCmd()
{
    delete ui;
}
void MainCmd::setssh(SSH *p_ssh)
{
    ssh = p_ssh;
}

void MainCmd::on_Edit_cmd_textChanged()
{

}
void MainCmd::keyPressEvent(QKeyEvent *event)
{
    qDebug("%d",event->key());

}

void MainCmd::on_Edit_cmd_selectionChanged()
{

}


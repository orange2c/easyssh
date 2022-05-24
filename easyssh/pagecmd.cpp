#include "pagecmd.h"
#include "ui_pagecmd.h"

PageCmd::PageCmd( SSH *p_ssh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PageCmd),
    ssh( p_ssh )
{
    ui->setupUi(this);
    shadow = new Shadow();
    connect( ssh, SIGNAL(shell_output(QString)), shadow, SLOT(ssh_message(QString)) );
    connect( shadow, SIGNAL(show(QString&,bool)), SLOT(update_cmd(QString&,bool)) );
}

PageCmd::~PageCmd()
{
    delete ui;
}


void PageCmd::keyPressEvent(QKeyEvent *event)
{
    qDebug("last=%d,now=%d", last_key, event->key());
    int k = event->key();
    int value = -1;
    QByteArray qb;
    if( Qt::Key_A <= k  &&  k <= Qt::Key_Z )
    {
        if( last_key == Qt::Key_Shift )
            value = k;
        else
            value = k + ('a'-'A');
    }
    switch ( k ) {

    case Qt::Key_Return:
        qDebug(" 回车按下 ");
        ssh->write( "\n" );
        break;

//    case Qt::Key_:
//        qDebug(" 回车按下 ");
//        ssh->write( "\n" );
//        break;

    default:
        break;
    }

    if( value > -1 )
    {
        qb.append(value);
//        ui->Edit_cmd->insertPlainText(qb.data());
        ssh->write(qb.data());
    }
    last_key = k;
}
void PageCmd::shell_output( QString data )
{
    ui->Edit_cmd->insertPlainText(data);
    ui->Edit_cmd->moveCursor(QTextCursor::End);
}
void PageCmd::update_cmd(QString &text, bool to_end)
{
    ui->Edit_cmd->clear();
    ui->Edit_cmd->insertPlainText(text);
    if( to_end )
        ui->Edit_cmd->moveCursor(QTextCursor::End);
}

void PageCmd::on_Button1_clicked()
{
    ssh->write("\r\n");
}

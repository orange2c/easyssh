#include "pagecmd.h"
#include "ui_pagecmd.h"

PageCmd::PageCmd( SSH *p_ssh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PageCmd),
    ssh( p_ssh )
{
    ui->setupUi(this);
    connect( ssh, SIGNAL(shell_output(QString)), SLOT(shell_output(QString)) );

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

    if( 33 <= k  &&  k <= 126 )
    {
        value = k;
    }

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
    default:
        break;
    }

    if( value > -1 )
    {
        qb.append(value);
//        ui->Edit_show->insertPlainText(qb.data());
        ssh->write(qb.data());
    }
    last_key = k;
}
void PageCmd::shell_output( QString data )
{
    ui->Edit_show->insertPlainText(data);
    ui->Edit_show->moveCursor(QTextCursor::End);
}
void PageCmd::update_cmd(QString &text, bool to_end)
{
    ui->Edit_show->clear();
    ui->Edit_show->insertPlainText(text);
    if( to_end )
        ui->Edit_show->moveCursor(QTextCursor::End);
}
void PageCmd::ssh_write( QString cmd )
{
    ssh->write( cmd );
}

void PageCmd::on_Button1_clicked()
{
    ssh->write("\n");
}

void PageCmd::on_Edit_write_textChanged()
{
    QString cmd_data = ui->Edit_write->toPlainText();
    QTextCursor docCursor = ui->Edit_write->textCursor();
    int pos = docCursor.position();
    qDebug("lastpos=%d,pos=%d", last_cmd_pos, pos);
    if( last_cmd_text.count() < cmd_data.count() && pos > last_cmd_pos ) //有新增数据
    {
        QString new_data = cmd_data.mid( last_cmd_pos, (pos-last_cmd_pos) );
        qDebug("检测到新数据pos=%s", qPrintable( new_data ));
        ssh->write( new_data );
        if( new_data.at( new_data.count()-1 ) == '\n' )
        {
            ui->Edit_write->clear();
            qDebug("回车");

            last_cmd_text.clear();
            last_cmd_pos = 0;
            return;
        }
    }
    if( last_cmd_text.count() > cmd_data.count() && pos < last_cmd_pos )//有数据被删除
    {

        QByteArray byte;
        byte.append( 127 );
        QString char_del( byte );
        int del_count = last_cmd_pos - pos;
        for( ; del_count >0; del_count-- )
        {
            qDebug("发送删除");
            ssh->write( char_del );
        }
    }

    last_cmd_text = cmd_data;
    last_cmd_pos = pos;

}

void PageCmd::on_Edit_write_cursorPositionChanged()
{
    QString cmd_data = ui->Edit_write->toPlainText();
    QTextCursor docCursor = ui->Edit_write->textCursor();
    int pos = docCursor.position();

    if( last_cmd_text.count() < cmd_data.count() && pos > last_cmd_pos ) //有新增数据
        return;//由于数据新增导致触发光标移动，过滤掉
    if( last_cmd_text.count() > cmd_data.count() && pos < last_cmd_pos )
        return;//由于数据删除导致触发光标移动，过滤掉

    if( pos > last_cmd_pos ) //向右移动
    {
        QByteArray byte;
        byte.append( 39 );
        QString char_move( byte );
        int move_count = pos - last_cmd_pos ;
        for( ; move_count >0; move_count-- )
        {
            ssh->write( char_move );
        }
    }
    if( pos < last_cmd_pos ) //向左移动
    {
        QByteArray byte;
        byte.append( 37 );
        QString char_move( byte );
        int move_count = last_cmd_pos - pos ;
        for( ; move_count >0; move_count-- )
        {
            ssh->write( char_move );
        }
    }
    last_cmd_pos = pos;
}

#include "pagecmd.h"
#include "ui_pagecmd.h"

PageCmd::PageCmd( SSH *p_ssh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PageCmd),
    ssh( p_ssh )
{
    ui->setupUi(this);
    connect( ssh, SIGNAL(shell_output(QString)), SLOT(shell_output(QString)) );
    init_edit();
    ui->Edit_write->setFocus();

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
    QByteArray byte;
    byte.append( 0x1b );
    byte.append( 0x5b );
    byte.append( 'A' );//转义序列
    QString char_move( byte );
    ssh->write( char_move );
}

void PageCmd::init_edit()
{
    is_init_deit = true;

    ui->Edit_write->clear();
    ui->Edit_write->insertPlainText( "\n\n " );

    QTextCursor tmpCursor = ui->Edit_write->textCursor();
    tmpCursor.setPosition( 1 ); //设置初始位置
    ui->Edit_write->setTextCursor( tmpCursor );

    last_cmd_text = ui->Edit_write->toPlainText();
    QTextCursor docCursor = ui->Edit_write->textCursor();
    last_cmd_pos = docCursor.position();

    ui->log->append("init_deit执行");
    is_init_deit = false;

}

void PageCmd::on_Edit_write_textChanged() //输入框文本变动事件
{
    if( is_init_deit )
        return;
    ui->log->append("输入框文本变动事件 发生");
    QString cmd_data = ui->Edit_write->toPlainText();

    QTextCursor docCursor = ui->Edit_write->textCursor();
    int pos = docCursor.position();
    qDebug("lastpos=%d,pos=%d", last_cmd_pos, pos);

    //有新增数据
    if( last_cmd_text.count() < cmd_data.count() && pos > last_cmd_pos )
    {
        QString new_data = cmd_data.mid( last_cmd_pos, (pos-last_cmd_pos) );
        qDebug("检测到新数据pos=[%s]", qPrintable( new_data ));
        ui->log->append( "新数据："+ new_data );
        ssh->write( new_data );
        if( new_data.at( new_data.count()-1 ) == '\n' )
        {
            ui->Edit_write->clear();
            qDebug("回车");
            init_edit();
            return;
        }

    }

    //有数据被backspace删除
    if( last_cmd_text.count() > cmd_data.count() && pos < last_cmd_pos )
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
    if( is_init_deit )
        return; //如果是init_edit在设置预置字符串时触发

    QString cmd_data = ui->Edit_write->toPlainText();
    if( last_cmd_text.count() != cmd_data.count())
        return; //由于文本变动导致的光标移动，过滤掉

    QTextCursor docCursor = ui->Edit_write->textCursor();
    int pos = docCursor.position();

    QByteArray key_byte; //方向键的转义序列共三位，前两位固定，第三位表示方向
    key_byte.append( 0x1b );
    key_byte.append( 0x5b );
    int move_count = 0 ;//左右方向可以设置次数
    if( pos == 0 || (pos==1 && last_cmd_pos>2 )  )
    {
        key_byte.append( 'A' );//转义序列
        move_count = 1 ;
        ui->log->append( "方向键上" );
    }
    else if( pos == cmd_data.count() )
    {
        key_byte.append( 'B' );//转义序列
        move_count = 1 ;
        ui->log->append( "方向键下" );
    }
    //方向键向右移动
    else if( pos > last_cmd_pos )
    {
        key_byte.append( 'C' );//转义序列
        move_count = pos - last_cmd_pos ;
        ui->log->append( "方向键右" );
    }
    //方向键向左移动
    else if( pos < last_cmd_pos )
    {
        key_byte.append( 'D' );//转义序列
        move_count = last_cmd_pos - pos ;
        ui->log->append( "方向键左" );
    }
     else //如果不是四个按键，则直接退出
        return;

     QString char_move( key_byte );
     for( ; move_count >0; move_count-- )
     {
         ssh->write( char_move );
     }
    last_cmd_pos = pos;
}

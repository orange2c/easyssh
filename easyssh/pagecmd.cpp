#include "pagecmd.h"
#include "ui_pagecmd.h"

PageCmd::PageCmd( SSH *p_ssh, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PageCmd),
    ssh( p_ssh )
{
    ui->setupUi(this);
    connect( ssh, SIGNAL(shell_output(QString)), SLOT(shell_output(QString)) );

    ui->Edit_write->setFocus();

    ecursor = new ECUROSR( ui->Edit_write );
    connect( ecursor, SIGNAL(cursor_change(int,int,int)), SLOT(ecursor_change(int,int,int)) );
    ecursor->signal_enable( true );

    etext = new ETEXT( ui->Edit_write );
    connect( etext, SIGNAL(text_change(int,bool,int,QString)), SLOT(etext_change(int,bool,int,QString)) );

    eshadow = new ESHADOW( ui->Edit_write, ui->Edit_show );
    eshadow->Edit_original = ui->Edit_show2;
    eshadow->Edit_hex = ui->ascii;
//    connect( ssh, SIGNAL(shell_output(QString)), eshadow, SLOT(shell_listen(QString))   );

}

PageCmd::~PageCmd()
{
    delete ui;
    delete ecursor;
    delete etext;
    delete eshadow;
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
        ssh->write(qb.data());
    }
    last_key = k;
}



//void PageCmd::eshow_pos_change(int relative)
//{
//    QString cmd_data = ui->Edit_show->toPlainText();
//    QTextCursor docCursor = ui->Edit_show->textCursor();
//    int now_pos = docCursor.position();  //获取当前pos位置
//    ui->log->append( "当前pos位置："+ QString::number(now_pos) );
//    now_pos += relative;
//    if( now_pos < 0 ) now_pos = 0;
//    if( now_pos > cmd_data.count() ) now_pos = cmd_data.count() ;
//    QTextCursor move = ui->Edit_show->textCursor();
//    move.setPosition( now_pos );
//    ui->Edit_show->setTextCursor( move );
//}
//void PageCmd::eshow_row_change(int relative)
//{
//    QTextDocument *doc = ui->Edit_show->document();
//    QTextCursor docCursor = ui->Edit_show->textCursor();

//    int row_now = docCursor.blockNumber();
//    row_now += relative;
//    if( row_now < 0 ) row_now = 0;
//    if( row_now > doc->blockCount() ) row_now = doc->blockCount();


//    QTextBlock block = doc->findBlockByNumber(row_now);
//    ui->Edit_show->setTextCursor( QTextCursor(block) );


//}



//接收ssh信息的槽
void PageCmd::shell_output( QString data )
{
    ecursor->signal_enable( false );
    etext->signal_enable( false );

    eshadow->shell_listen( data );

    ecursor->signal_enable( true );
    etext->signal_enable( true );
    etext->save_now();

}

void PageCmd::on_Button1_clicked()
{
//    QByteArray byte;
//    byte.append( 0x1b );
//    byte.append( 0x5b );
//    byte.append( 'A' );//转义序列
//    QString char_move( byte );
//    ssh->write( char_move );
    ssh->write( "vim VULTR_KEY.txt" );

}



void PageCmd::ecursor_change( int row, int column, int pos )
{

    qDebug("相对移动：r:%d c:%d pos:%d", row, column, pos );

    QByteArray key_byte; //方向键的转义序列共三位，前两位固定，第三位表示方向
    key_byte.append( 0x1b );
    key_byte.append( 0x5b );
    int move_count = 0 ;//左右方向可以设置次数

    if( row<0 )
    {
        key_byte.append( 'A' );//转义序列
        move_count = 1 ;
        ui->log->append( "方向键上" );
    }
    else if( row>0 )
    {
        key_byte.append( 'B' );//转义序列
        move_count = 1 ;
        ui->log->append( "方向键下" );

        ecursor->signal_enable( false );
//        ecursor->move_rel( -pos );
        ecursor->signal_enable( true );
    }
    //方向键向右移动
    else if( column > 0 )
    {
        key_byte.append( 'C' );//转义序列
        move_count = column ;
        ui->log->append( "方向键右" );
    }
    //方向键向左移动
    else if( column < 0 )
    {
        key_byte.append( 'D' );//转义序列
        move_count = -column ;
        ui->log->append( "方向键左" );
        left_todel_count ++ ;

    }

//    if( row != 0 ) //限制光标只能在这一行内
//    {
//        ecursor->signal_enable( false );
//        ecursor->move_rel( -pos );
//        ecursor->signal_enable( true );
//    }


    QString char_move( key_byte );
    for( ; move_count >0; move_count-- )
    {
        ssh->write( char_move );
    }
}

void PageCmd::etext_change(int delete_count, bool is_backspace, int add_count, QString new_str)
{
    if( is_backspace )
    {
        qDebug("左删除%d",delete_count );
    }
    else
    {
        qDebug("右删除%d",delete_count );
    }
    char byte_right[] = { 0x1b, 0x5b, 'C'};
    QByteArray move_right( byte_right, 3 );
    QByteArray byte;
    byte.append( 0x8 );
    QString char_del( byte );
    for( ; delete_count >0; delete_count-- )
    {
        qDebug("发送删除");
        if( !is_backspace )
            ssh->write( move_right );
        ssh->write( char_del );
    }

    qDebug( "新增%d:%s", add_count, qPrintable( new_str ) );
//    ssh->write( new_str );

}

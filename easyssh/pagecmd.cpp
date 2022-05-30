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

    ecursor = new ECUROSR( ui->Edit_write );
    connect( ecursor, SIGNAL(cursor_change(int,int,int)), SLOT(ecursor_change(int,int,int)) );
    ecursor->signal_enable( true );

    etext = new ETEXT( ui->Edit_write );
    connect( etext, SIGNAL(text_change(int,bool,int,QString)), SLOT(etext_change(int,bool,int,QString)) );

    eshadow = new QTextEdit();
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



void PageCmd::eshow_pos_change(int relative)
{
    QString cmd_data = ui->Edit_show->toPlainText();
    QTextCursor docCursor = ui->Edit_show->textCursor();
    int now_pos = docCursor.position();  //获取当前pos位置
    ui->log->append( "当前pos位置："+ QString::number(now_pos) );
    now_pos += relative;
    if( now_pos < 0 ) now_pos = 0;
    if( now_pos > cmd_data.count() ) now_pos = cmd_data.count() ;
    QTextCursor move = ui->Edit_show->textCursor();
    move.setPosition( now_pos );
    ui->Edit_show->setTextCursor( move );
}
void PageCmd::eshow_row_change(int relative)
{
    QTextDocument *doc = ui->Edit_show->document();
    QTextCursor docCursor = ui->Edit_show->textCursor();

    int row_now = docCursor.blockNumber();
    row_now += relative;
    if( row_now < 0 ) row_now = 0;
    if( row_now > doc->blockCount() ) row_now = doc->blockCount();


    QTextBlock block = doc->findBlockByNumber(row_now);
    ui->Edit_show->setTextCursor( QTextCursor(block) );


}
void PageCmd::eshow_backspace( int count )
{
    QString show_data = ui->Edit_write->toPlainText();
    QTextCursor docCursor = ui->Edit_write->textCursor();
    int pos = docCursor.position() ;
    show_data.remove( pos-count, count );


//    ui->Edit_write->clear();
//    ui->Edit_write->insertPlainText( show_data );
//    QTextCursor newCursor = ui->Edit_write->textCursor();
//    newCursor.deleteChar();
//    newCursor.setPosition( pos  );
//    ui->Edit_write->setTextCursor( newCursor );
}


//接收ssh信息的槽
void PageCmd::shell_output( QString data )
{
//    QTextEdit *show = ui->Edit_write;

    QTextEdit *show = eshadow ;

    ecursor->signal_enable( false );
    etext->signal_enable( false );

    int i = 0;
    while( i < last_send.count() )
    {
        if( last_send.at( i )  !=  data.at( i ) )
            break;
        i++;
    }
    last_send.clear();

    for( ; i<data.count(); i++ )
    {
        switch( data.at(i).toLatin1() ) //
        {
        case 0x7: //7，振铃
//            show->moveCursor(QTextCursor::End);
            continue;
        case 0x8: //8，退格（方向键左）
            eshow_pos_change(-1);
            continue;
        case '\r':
            if( data.at(i+1).toLatin1() == '\n' )
            {
                i += 1;
                show->insertPlainText( "\n" );
                edit_write2show();
                continue;
            }

        case 0x1b: //ESC 一堆转义序列的起始
            if( data.at(i+1).toLatin1() == 0x5b)
            {
                if( data.at(i+2).toLatin1() == 'C' ) //向右移动一格
                {
                    i+=2;
                    eshow_pos_change(+1);
                    continue;
                }
                if( data.at(i+2).toLatin1() == 0x4b ) //删除
                {
                    i+=2;
                    eshow_backspace();
                }
            }
            else
                break; //直接退出
            continue;

        }
        if( left_todel_count >0 )
        {
//            eshow_delete( left_todel_count );
            left_todel_count = 0;
        }
        show->insertPlainText( data.at(i) );
    }


    ui->Edit_show2->insertPlainText(data);
    ui->Edit_show2->moveCursor(QTextCursor::End);

    QByteArray byte = data.toUtf8();
    for( int i=0; i< byte.count(); i++ )
    {
        ui->ascii->insertPlainText( "十进制:"+ QString::number( byte.at(i) )+ "  Hex:"+ QString::number( byte.at(i), 16 )+ ":"+byte.at(i)+ '\n' );
    }
    ui->ascii->moveCursor(QTextCursor::End);



    QString text = eshadow->toHtml();

    ui->Edit_write->clear();
    ui->Edit_write->insertHtml( text );
    ui->Edit_show->moveCursor( QTextCursor::End );


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
    eshow_row_change(-1);
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

//输入框文本变动事件
void PageCmd::on_Edit_write_textChanged()
{

//    if( is_init_deit )
//        return;
//    ui->log->append("输入框文本变动事件 发生");
//    QString cmd_data = ui->Edit_write->toPlainText();

//    QTextCursor docCursor = ui->Edit_write->textCursor();
//    int pos = docCursor.position();
//    qDebug("lastpos=%d,pos=%d", last_cmd_pos, pos);

//    //有新增数据
//    if( last_cmd_text.count() < cmd_data.count() && pos > last_cmd_pos )
//    {
//        QString new_data = cmd_data.mid( last_cmd_pos, (pos-last_cmd_pos) );
//        qDebug("检测到新数据pos=[%s]", qPrintable( new_data ));
//        ui->log->append( "新数据："+ new_data );
//        ssh->write( new_data );
//        if( new_data.at( new_data.count()-1 ) == '\n' )
//        {
//            ui->Edit_write->clear();
//            ui->Edit_show->moveCursor(QTextCursor::End);
//            qDebug("回车");
//            init_edit();
//            return;
//        }
//    }

//    //有数据被backspace删除
//    if( last_cmd_text.count() > cmd_data.count() && pos < last_cmd_pos )
//    {

//        QByteArray byte;
//        byte.append( 0x8 );
//        QString char_del( byte );
//        int del_count = last_cmd_pos - pos;
//        for( ; del_count >0; del_count-- )
//        {
//            qDebug("发送删除");
//            ssh->write( char_del );
//        }
//    }
//    //有数据被delete删除
//    if( last_cmd_text.count() > cmd_data.count() && pos == last_cmd_pos )
//    {

//        QByteArray byte;
//        byte.append( 127 );
//        QString char_del( byte );
//        int del_count = last_cmd_text.count() - cmd_data.count();
//        for( ; del_count >0; del_count-- )
//        {
//            qDebug("发送删除");
//            ssh->write( char_del );
//        }
//    }

//    last_cmd_text = cmd_data;
//    last_cmd_pos = pos;

}

//输入框光标移动事件
void PageCmd::on_Edit_write_cursorPositionChanged()
{
//    if( is_init_deit )
//        return; //如果是init_edit在设置预置字符串时触发

//    QString cmd_data = ui->Edit_write->toPlainText();
//    if( last_cmd_text.count() != cmd_data.count() )
//        return; //由于文本变动导致的光标移动，过滤掉
//    if( cmd_data.count() == 0 )
//        return; //由于文本框clear触发，

//    QTextCursor docCursor = ui->Edit_write->textCursor();
//    int pos = docCursor.position();

//    QByteArray key_byte; //方向键的转义序列共三位，前两位固定，第三位表示方向
//    key_byte.append( 0x1b );
//    key_byte.append( 0x5b );
//    int move_count = 0 ;//左右方向可以设置次数
//    if( (pos==0 && last_cmd_pos>=1 )  )
//    {
//        key_byte.append( 'A' );//转义序列
//        move_count = 1 ;
//        ui->log->append( "方向键上" );
//    }
//    else if( pos==cmd_data.count() && pos!=0 )
//    {
//        key_byte.append( 'B' );//转义序列
//        move_count = 1 ;
//        ui->log->append( "方向键下" );
//    }
//    //方向键向右移动
//    else if( pos > last_cmd_pos )
//    {
//        key_byte.append( 'C' );//转义序列
//        move_count = pos - last_cmd_pos ;
//        ui->log->append( "方向键右" );
//    }
//    //方向键向左移动
//    else if( pos < last_cmd_pos )
//    {
//        key_byte.append( 'D' );//转义序列
//        move_count = last_cmd_pos - pos ;
//        ui->log->append( "方向键左" );
//        left_todel_count ++ ;

//    }
//     else //如果不是四个按键，则直接退出
//        return;

//     QString char_move( key_byte );
//     for( ; move_count >0; move_count-- )
//     {
//         ssh->write( char_move );
//     }
//    last_cmd_pos = pos;
}

void PageCmd::edit_write2show()
{
    QString text = eshadow->toHtml();
    ui->Edit_show->insertHtml( text );
    ui->Edit_write->clear();
    ui->Edit_show->moveCursor( QTextCursor::End );

    eshadow->clear();

}

void PageCmd::ecursor_change( int row, int column, int pos )
{

    qDebug("相对移动：r:%d c:%d pos:%d", row, column, pos );

    QByteArray key_byte; //方向键的转义序列共三位，前两位固定，第三位表示方向
    key_byte.append( 0x1b );
    key_byte.append( 0x5b );
    int move_count = 0 ;//左右方向可以设置次数

    if( row<0 && pos<-1 )
    {
        key_byte.append( 'A' );//转义序列
        move_count = 1 ;
        ui->log->append( "方向键上" );
    }
    if( row>0 && pos>1 )
    {
        key_byte.append( 'B' );//转义序列
        move_count = 1 ;
        ui->log->append( "方向键下" );
    }
    //方向键向右移动
    if( column > 0 )
    {
        key_byte.append( 'C' );//转义序列
        move_count = column ;
        ui->log->append( "方向键右" );
    }
    //方向键向左移动
    if( column < 0 )
    {
        key_byte.append( 'D' );//转义序列
        move_count = -column ;
        ui->log->append( "方向键左" );
        left_todel_count ++ ;

    }

    if( row != 0 ) //限制光标只能在这一行内
    {
        ecursor->signal_enable( false );
        ecursor->move_rel( -pos );
        ecursor->signal_enable( true );
    }


    QString char_move( key_byte );
    for( ; move_count >0; move_count-- )
    {
        ssh->write( char_move );
    }
}

void PageCmd::etext_change(int delete_count, bool is_backspace, int add_count, QString new_str)
{
    if( is_backspace )
        qDebug("左删除%d",delete_count );
    else
        qDebug("右删除%d",delete_count );
    qDebug( "新增%d:%s", add_count, qPrintable( new_str ) );


    QByteArray byte;
    byte.append( 0x8 );
    QString char_del( byte );


    last_send.append( new_str );

    for( ; delete_count >0; delete_count-- )
    {
        qDebug("发送删除");
        ssh->write( char_del );
    }
    ssh->write( new_str );



}

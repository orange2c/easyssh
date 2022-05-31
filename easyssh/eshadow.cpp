#include "eshadow.h"

ESHADOW::ESHADOW( QTextEdit *write, QTextEdit *show, QObject *parent) :
    QObject(parent), Edit_write(write), Edit_show(show)
{
    Edit_shadow = new QTextEdit();
}
ESHADOW::~ESHADOW()
{
    delete Edit_shadow;
}

void ESHADOW::updata_show()
{

    qDebug("updata_show开始");
    qDebug("解析：%s", qPrintable( Edit_shadow->toHtml() ) );
    Edit_write->clear();
    Edit_show->insertHtml( Edit_shadow->toHtml() );

    Edit_show->moveCursor( QTextCursor::End );
    Edit_shadow->clear();

}
void ESHADOW::updata_wirte()
{
    Edit_write->clear();
    Edit_write->insertHtml( Edit_shadow->toHtml() );


    QTextCursor shadowCursor = Edit_shadow->textCursor();
    int shadow_pos = shadowCursor.position();  //获取当前pos位置


    QTextCursor writeCursor = Edit_write->textCursor();
    writeCursor.setPosition( shadow_pos );
    Edit_write->setTextCursor( writeCursor );

}



void ESHADOW::shadow_pos_change(int relative)
{
    QTextCursor docCursor = Edit_shadow->textCursor();
    int now_pos = docCursor.position();  //获取当前pos位置

    now_pos += relative;

    int count = Edit_shadow->toPlainText().count();
    if( now_pos < 0 ) now_pos = 0;
    if( now_pos > count ) now_pos = count ;

    docCursor.setPosition( now_pos );
    Edit_shadow->setTextCursor( docCursor );
}

void ESHADOW::shadow_backspace( int count )
{
    QTextCursor docCursor = Edit_shadow->textCursor();
    for( ; count >0; count-- )
        docCursor.deletePreviousChar();
}
void ESHADOW::shadow_delete( int count )
{
    shadow_pos_change( count );
    QTextCursor docCursor = Edit_shadow->textCursor();
    for( ; count >0; count-- )
        docCursor.deletePreviousChar();
}
void ESHADOW::shadow_delete_blockEND()
{
    QTextCursor docCursor = Edit_shadow->textCursor();
    QTextDocument *doc = Edit_shadow->document();
    QTextBlock docBlock  =  doc->findBlock( docCursor.position() );
    int line_count = docBlock.text().count() - docCursor.positionInBlock() ;
    shadow_delete( line_count );


}

void ESHADOW::shell_listen(QString data)
{

    Edit_hex->append( "接收新消息----------:\n" );

    bool is_enter = false;

    for( int i = 0; i<data.count(); i++ )
    {
        switch( data.at(i).toLatin1() ) //
        {
        case 0x7: //7，振铃
//            show->moveCursor(QTextCursor::End);
            continue;
        case 0x8: //8，退格（方向键左）
            shadow_pos_change( -1 );
            continue;
        case '\r':
            if( data.at(i+1).toLatin1() == '\n' )
            {
                i += 1;

//                updata_show();
                Edit_shadow->insertPlainText( "\n" );
                is_enter = true;
                continue;
            }

        case 0x1b: //ESC 一堆转义序列的起始
            if( data.at(i+1).toLatin1() == 0x5b)
            {
                if( data.at(i+2).toLatin1() == 'C' ) //向右移动一格
                {
                    i+=2;
                    shadow_pos_change( +1 );
                    continue;
                }
                if( data.at(i+2).toLatin1() == 0x4b ) //从光标删除到行尾
                {
                    i+=2;
                    shadow_delete_blockEND();
                }
            }
            else
                break; //直接退出
            continue;

        }

        Edit_shadow->insertPlainText( data.at(i) );

    }


    Edit_original->insertPlainText(data);
    Edit_original->moveCursor(QTextCursor::End);

    QByteArray byte = data.toUtf8();
    for( int i=0; i< byte.count(); i++ )
    {
        Edit_hex->insertPlainText( "十进制:"+ QString::number( byte.at(i) )+ "  Hex:"+ QString::number( byte.at(i), 16 )+ ":"+byte.at(i)+ '\n' );
    }
    if( is_enter )
    {
        Edit_shadow->insertPlainText( "\n" );
        shadow_pos_change( -1 );
    }
    Edit_hex->moveCursor(QTextCursor::End);
    Edit_show->moveCursor( QTextCursor::End );

    updata_wirte();
}

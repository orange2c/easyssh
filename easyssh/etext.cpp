#include "etext.h"

ETEXT::ETEXT( QTextEdit *text_edit, QObject *parent) :
    QObject(parent), Edit( text_edit )
{
    connect( Edit, SIGNAL(textChanged()), SLOT(edit_text_change()) );
    connect( Edit, SIGNAL(cursorPositionChanged()), SLOT(edit_cursor_change()) );
    Doc = Edit->document();
//    save_text = new QString( Edit->toPlainText() );
}
ETEXT::~ETEXT()
{
    if( save_text != NULL )
        delete save_text;
}

void ETEXT::save_now( QString *text )
{
    QTextDocument *doc = Edit->document();

    if( save_text != NULL )
        delete save_text;

    if( text == NULL )
        save_text = new QString( doc->toRawText() );
    else
        save_text = text;

}
void ETEXT::edit_cursor_change()
{
    int text_count = Edit->toPlainText().count();
    if( text_count != save_text->count() )
        return; //如果是由于文本变动导致的光标变动，则不操作
    QTextCursor cursor = Edit->textCursor();
    save_pos = cursor.position();

}

void ETEXT::edit_text_change()
{


//    QString *now_text = new QString( Edit->toPlainText() );
    QTextDocument *doc = Edit->document();
    QString *now_text = new QString( doc->toPlainText() );

    QTextCursor cursor = Edit->textCursor();
    int pos = cursor.position();

    int now_count = now_text->count();
    int last_count = save_text->count();


    int same_left = 0;

    int same_now_right = 0;
    int same_last_right = 0;

    //寻找输入框内的字符串与保存的字符串，从左往右与从右往左各有多少个连续相同字符
    if( (now_count!=0) && (last_count!=0) ) //必须两串大小都不为0
    {
        int tmp_now ;
        int tmp_last ;
        while( 1 )
        {
            tmp_now = now_count - same_now_right - 1;
            tmp_last = last_count - same_last_right - 1;
            if(  tmp_now < pos )
                break;
            if(  tmp_last <= 0 )
                break;
            if( now_text->at(tmp_now) != save_text->at(tmp_last) )
                break;
            same_now_right++;
            same_last_right++;
        }
        while( 1 )
        {
            if( same_left   >= pos )
                break;
            if( same_left   >=   (last_count - same_last_right) )
                break;
            if( now_text->at(same_left) != save_text->at( same_left ) )
                break;
            same_left++;
        }
    }

    int delete_count = 0;
    bool is_backspace = true;

    int last_count_same = same_left + same_last_right; //last中共有几个now中相同字符
    if( last_count_same < last_count ) //last中有now没有的字符
    {
        delete_count = last_count - last_count_same; //last比now多几个不存在字符，就删几次
//        qDebug( "save=%d,pos=%d", save_pos, pos );
        if( pos == save_pos ) //如果光标位置跟之前一样，则是右删除
        {
            is_backspace = false;
        }
    }

    int add_count = pos - same_left;
    QString new_str( now_text->mid( same_left, add_count ) );

//    qDebug( "now=%s", qPrintable( *now_text ) );
    save_now( now_text );
    if(  is_signal_enable )
        emit text_change( delete_count, is_backspace, add_count, new_str );

}

void ETEXT::signal_enable(bool flag)
{
    is_signal_enable = flag;
}

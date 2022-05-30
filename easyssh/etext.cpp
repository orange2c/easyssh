#include "etext.h"

ETEXT::ETEXT( QTextEdit *text_edit, QObject *parent) :
    QObject(parent), Edit( text_edit )
{
    connect( Edit, SIGNAL(textChanged()), SLOT(edit_text_change()) );
    Doc = Edit->document();
}
ETEXT::~ETEXT()
{
    if( save_text != NULL )
        delete save_text;
}

void ETEXT::save_now( QString *text )
{
    if( save_text != NULL )
        delete save_text;

    if( text == NULL )
        save_text = new QString( Edit->toPlainText() );
    else
        save_text = text;

}

void ETEXT::edit_text_change()
{
    if( ! is_signal_enable )
        return;

    QString *now_text = new QString( Edit->toPlainText() );
    QTextCursor cursor = Edit->textCursor();
    int pos = cursor.position();

    int now_count = now_text->count();
    int last_count = save_text->count();


    int same_left = 0;

    int same_now_right = 0;
    int same_last_right = 0;

    //last从左往右与now比对是否相同
    //last从右往左与now比对是否相同
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


    if( (now_count==last_count)  )
    {
        //相同字符数的复制粘贴
        if( (same_left < pos)  ) //左相同小于pos
        {
            qDebug("删除:%s", qPrintable( save_text->mid( same_left,  pos - same_left ) )  );
            qDebug("粘贴：%s", qPrintable( now_text->mid( same_left, pos - same_left ) ) );
        }
        else
            qDebug("误判，全相同");
    }
    else if( now_count > last_count )
    {
        if( (same_left +  same_now_right)  ==  last_count )
        {
            int change_count = pos - same_left;
            qDebug("纯新增文本：%s", qPrintable( now_text->mid( same_left, change_count ) ) );
        }

    }


    qDebug( "last_count=%d,now_count=%d", last_count, now_count );
    qDebug( "same_left=%d,last_right=%d,now_right=%d", same_left, same_last_right, same_now_right );
    qDebug( "pos=%d", pos );

//    qDebug("文本变动");
    save_now( now_text );


}

void ETEXT::signal_enable(bool flag)
{
    is_signal_enable = flag;
}

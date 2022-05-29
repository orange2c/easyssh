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

    int now_right = 0;
    int last_right = 0;

    //last从左往右与now比对是否相同
    //last从右往左与now比对是否相同
    if( now_count!=0 && last_count!=0 ) //必须两串大小都不为0
    {
        while( 1 )
        {
            if( now_text->at(same_left) != save_text->at( same_left ) )
                break;
            same_left++;
            if( same_left >= pos)
                break;
            if( same_left >= pos)
                break;
        }
        int tmp_n ;
        int tmp_l ;
        while( 1 )
        {
            tmp_n = now_count - now_right - 1;
            tmp_l = last_count - last_right - 1;
            if( tmp_n < pos)
                break;
            if( tmp_l < pos)
                break;

            if( now_text->at(tmp_n) != save_text->at(tmp_l) )
                break;

            now_right++;
            last_right++;
        }
    }
    qDebug("ces");

    if( (now_count==last_count) && (same_left==now_count)  )
    {
        qDebug("误判，全相同");
    }

//    if(  )
//    if( (now_left+1 == now_right) && (last_left+1 == last_right)  ) //两者相同，不做操作
//    {

//    }
//    else if( (last_left+1 == last_right) &&  (now_right == pos) )//如果last的都存在，且新增的刚好到pos则为纯新增
//    {
//        qDebug("纯新增");
//        emit text_add( now_text->mid( last_left, last_right-last_left ) );
//        //
//    }

//    else if( (now_left+1 == now_right) &&  (last_left+1 != last_right) )//如果now的都存在，且last有多的字符
//    {
//        qDebug("删除");
//        if( (last_left < pos) && (last_right == pos) )
//        {
//            qDebug( "左删除" );
//            emit text_del( pos-last_left, true );
//        }
//        else if( (last_left == pos) && (last_right > pos) )
//        {
//            qDebug( "右删除" );
//            emit text_del( last_right-pos, false );
//        }
//    }




    qDebug( "same_left=%d,now_right=%d", same_left, now_right );
    qDebug( "same_left=%d,last_right=%d", same_left, last_right );

    qDebug("文本变动");
    save_now( now_text );


}

void ETEXT::signal_enable(bool flag)
{
    is_signal_enable = flag;
}

#include "shadow.h"
/*
交互式窗口的影子
当发生文本改变事件与坐标改变事件时，调用本类，负责进行计算
*/
Shadow::Shadow(QObject *parent) : QObject(parent)
{

}

void Shadow::ssh_message(QString text)
{
    save_history.append( save_result );
//    save_result.clear();
    save_result.append( text );
    emit show( save_result, true );
    qDebug("ssh message 新消息");
}
void Shadow::change_pos( int pos )
{

}
void Shadow::change_text( QString text )
{

}

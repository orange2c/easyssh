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

void ETEXT::update_start()
{
    QTextCursor cursor = Edit->textCursor();
    pos_start = cursor.position();
    block_start = cursor.blockNumber();
    if( save_text != NULL )
        delete save_text;
    QString text = Edit->toPlainText();
    save_text = new QString( text.mid( pos_start, text.count()-pos_start ) );

}

void ETEXT::edit_text_change()
{
    if( ! is_signal_enable )
        return;
    QString text = Edit->toPlainText();
    QString *now_text = new QString( text.mid( pos_start, text.count()-pos_start ) );




    int diff_start = 0;
    int diff_end = now_text->count() -1 ;



    qDebug("文本变动");



}

void ETEXT::signal_enable(bool flag)
{
    is_signal_enable = flag;
}

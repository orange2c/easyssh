#include "ecurosr.h"

ECUROSR::ECUROSR( QTextEdit *text_edit, QObject *parent) :
    QObject(parent), Edit( text_edit )
{
    connect( Edit, SIGNAL(cursorPositionChanged()), SLOT(edit_curosr_change()) );
    Doc = Edit->document();
}

void ECUROSR::move_rel(int pos)
{
    QTextCursor move = Edit->textCursor();
    pos += move.position();
    int max_pos = Edit->toPlainText().count();
    if( pos <= 0 ) pos = 0;
    if( pos > max_pos ) pos = max_pos;
    move.setPosition( pos );
    Edit->setTextCursor( move );

}


inline void ECUROSR::save_rc(int row, int column)
{
    save_row[0] = save_row[1];
    save_row[1] = row;
    save_col[0] = save_col[1];
    save_col[1] = column;
}
void ECUROSR::signal_enable(bool flag)
{
    is_signal_enable = flag;
}

void ECUROSR::edit_curosr_change()
{
    QTextCursor docCursor = Edit->textCursor();
    int pos = docCursor.position();

    save_rc( docCursor.blockNumber(), docCursor.positionInBlock() );

    int str_count = Doc->findBlockByNumber(save_row[1]).text().count();
    //待添加功能，识别最大col。vim等文本编辑器必需
    //如当前光标col为10，移动到下一行，该行只有3个字符，则光标col为3
    //此时再移动到新一行，该行字符数大于10，则光标col会变成10

    if( is_signal_enable )
        emit cursor_change( save_row[1]-save_row[0], save_col[1]-save_col[0], pos-save_pos );
    save_pos = pos;
}



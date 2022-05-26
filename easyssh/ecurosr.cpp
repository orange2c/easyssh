#include "ecurosr.h"

ECUROSR::ECUROSR( QTextEdit *text_edit, QObject *parent) :
    QObject(parent), Edit( text_edit )
{
    connect( Edit, SIGNAL(cursorPositionChanged()), SLOT(edit_curosr_change()) );
    Doc = Edit->document();
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
    qDebug("edit_curosr_change");
    QTextCursor docCursor = Edit->textCursor();
    int pos = docCursor.position();

    save_rc( docCursor.blockNumber(), docCursor.positionInBlock() );

    if( is_signal_enable )
        emit cursor_change( save_row[1]-save_row[0], save_col[1]-save_col[0], pos-save_pos );
    save_pos = pos;
}



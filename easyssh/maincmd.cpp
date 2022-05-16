#include "maincmd.h"
#include "ui_maincmd.h"

MainCmd::MainCmd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainCmd)
{
    ui->setupUi(this);

//    ui->Edit_cmd->insertPlainText("sss");
//    ui->Edit_cmd->insertPlainText("hello");
//    ssh->write("pwd" );

}

MainCmd::~MainCmd()
{
    delete ui;
}
void MainCmd::setssh(SSH *p_ssh)
{
    ssh = p_ssh;
}


void MainCmd::keyPressEvent(QKeyEvent *event)
{
    qDebug("last=%d,now=%d", last_key, event->key());
    int k = event->key();
    int value = -1;
    QByteArray qb;
    if( Qt::Key_A <= k  &&  k <= Qt::Key_Z )
    {
        if( last_key == Qt::Key_Shift )
            value = k;
        else
            value = k + ('a'-'A');
    }

    if( value > -1 )
    {
        qb.append(value);
        ui->Edit_cmd->insertPlainText(qb.data());
    }
    last_key = k;
}

void MainCmd::on_Edit_cmd_selectionChanged()
{
    ui->Edit_cmd->toPlainText();
}


void MainCmd::on_Edit_2_cursorPositionChanged()
{

}


void MainCmd::on_Edit_cmd_cursorPositionChanged()
{
    QTextCursor docCursor = ui->Edit_cmd->textCursor();

    if( docCursor.atBlockEnd() )
    {
//        docCursor.movePosition(QTextCursor::EndOfLine);
        ui->Edit_cmd->moveCursor(QTextCursor::End);
        qDebug("not atend");
    }

    qDebug( "pos in block=%d", docCursor.positionInBlock());

    qDebug("line:%d",docCursor.blockNumber());
    qDebug("text=%s", qPrintable( ui->Edit_cmd->document()->findBlockByLineNumber(docCursor.blockNumber() ).text()   ));

    QString text = ui->Edit_cmd->toPlainText();

    if( docCursor.atEnd() && last_pos<docCursor.position() )
    {
        qDebug("新输入字符%c",  text.at( text.size()-1 ) );
        ssh->write(QString( text.at( text.size()-1 )) );

         qDebug("新输入字符%c",  text.at( text.size()-1 ) );
        ui->Edit_show->insertPlainText( ssh->read() );
    }
    if( docCursor.blockNumber()>=1 && docCursor.positionInBlock() == 0 )
    {
        ssh->write( "\r\n" );
        ui->Edit_show->insertPlainText( ssh->read() );
    }
    //如果行数增加到1，且光标为0，则发送回车
    //如果


    last_block_number = docCursor.blockNumber();
    last_pos = docCursor.position();

}


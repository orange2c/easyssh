#ifndef PAGECMD_H
#define PAGECMD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextList>
#include <QTextCodec>
#include <QTextEdit>
#include "ssh.h"
#include "ecurosr.h"
#include "etext.h"
#include "eshadow.h"

namespace Ui {
class PageCmd;
}

class PageCmd : public QMainWindow
{
    Q_OBJECT

public:
    explicit PageCmd( SSH *p_ssh, QWidget *parent = 0);
    ~PageCmd();

protected:
    ECUROSR *ecursor;
    ETEXT *etext;
    ESHADOW *eshadow;

    //    int enter_pos = 0; //记录按下回车时，其所处位置


    int left_todel_count = 0; //ssh命令输入时，如果按左键，则下次会把右边几个字母也返回
    int last_key = 0; //按下组合键时会触发两次按键事件


    void init_edit(); //清空输入框，填入预置字符，设置各个变量
    void eshow_pos_change( int relative ); //让show窗口的光标，左右移动相对位置
    void eshow_row_change( int relative ); //让show窗口的光标，上下移动相对位置

    void eshow_delete_at( int pos );
    void eshow_backspace( int count = 1 ); //删除当前光标所在前一个字符



private slots:

    void on_Button1_clicked();

    void on_Edit_write_textChanged();
    void on_Edit_write_cursorPositionChanged();

    void shell_output( QString data );
    void ecursor_change( int row, int column, int pos );
    void etext_change( int delete_count, bool is_backspace, int add_count, QString new_str );



private:
    Ui::PageCmd *ui;
    SSH *ssh;

    void keyPressEvent(QKeyEvent *event); //检测按键按下事件
};

#endif // PAGECMD_H

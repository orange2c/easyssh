#ifndef PAGECMD_H
#define PAGECMD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextList>
#include <QTextCodec>
#include "ssh.h"

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
    int last_key = 0; //按下组合键时会触发两次按键事件
    QString cmd_show_text; //存储展示框内需要展示的字符串
    QString last_cmd_text; //存储文本框上一次存储的数据
    int last_cmd_pos = 0;  //存储文本框上一次的光标位置

    bool is_init_deit = false ; //init_edit会触发对应文本框事件，为了让事件内方便识别跳过
    bool is_edit = true; //true，则会把ssh返回值发送到输入框内，false则只发送到展示框

    void init_edit(); //清空输入框，填入预置字符，设置各个变量
    void eshow_pos_change( int relative ); //让show窗口的光标，左右移动相对位置
    void eshow_row_change( int relative ); //让show窗口的光标，上下移动相对位置

    void eshow_delete_at( int pos );
    void eshow_backspace(); //删除当前光标所在前一个字符

private slots:
    void shell_output( QString data );
    void on_Button1_clicked();

    void on_Edit_write_textChanged();
    void on_Edit_write_cursorPositionChanged();

private:
    Ui::PageCmd *ui;
    SSH *ssh;

    void keyPressEvent(QKeyEvent *event); //检测按键按下事件
};

#endif // PAGECMD_H

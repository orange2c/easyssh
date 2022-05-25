#ifndef PAGECMD_H
#define PAGECMD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextList>
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
    int last_key = 0; //按下组合键时会被拆成多次调用

    QString last_cmd_text; //存储文本框上一次存储的数据
    int last_cmd_pos = 0;  //存储上一次上一次的光标位置

    bool is_init_deit = false ; //init_edit会触发对应文本框事件，为了让事件内方便识别跳过

    void init_edit(); //清空输入框，填入预置字符，设置各个变量

private slots:
    void shell_output( QString data );
    void on_Button1_clicked();
    void update_cmd( QString &text, bool to_end );
    void ssh_write( QString cmd );

    void on_Edit_write_textChanged();
    void on_Edit_write_cursorPositionChanged();

private:
    Ui::PageCmd *ui;
    SSH *ssh;

    void keyPressEvent(QKeyEvent *event); //检测按键按下事件
};

#endif // PAGECMD_H

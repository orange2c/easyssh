#ifndef PAGECMD_H
#define PAGECMD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextList>
#include "ssh.h"
#include "shadow.h"

namespace Ui {
class PageCmd;
}

class PageCmd : public QMainWindow
{
    Q_OBJECT

public:
    explicit PageCmd( SSH *p_ssh, QWidget *parent = 0);
    ~PageCmd();

private slots:
    void shell_output( QString data );
    void on_Button1_clicked();
    void update_cmd( QString &text, bool to_end );
    void ssh_write( QString cmd );


    void on_Edit_write_textChanged();

private:
    Ui::PageCmd *ui;

    SSH *ssh;
    Shadow *shadow;

    int last_key = 0; //按下组合键时会被拆成多次调用
    int last_pos = 0;
    int last_block_number = 0;

    QString cmd_text; //存储当前窗口应该显示的内容

    int last_cmd_pos = 0;




    void keyPressEvent(QKeyEvent *event);
};

#endif // PAGECMD_H

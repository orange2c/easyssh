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

private slots:
    void shell_output( QString data );
    void on_Button1_clicked();

private:
    Ui::PageCmd *ui;

    SSH *ssh;

    int last_key = 0; //按下组合键时会被拆成多次调用
    int last_pos = 0;
    int last_block_number = 0;

    void keyPressEvent(QKeyEvent *event);
};

#endif // PAGECMD_H

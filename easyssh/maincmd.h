#ifndef MAINCMD_H
#define MAINCMD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextList>

namespace Ui {
class MainCmd;
}

class MainCmd : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainCmd(QWidget *parent = nullptr);
    ~MainCmd();

private slots:

    void on_Edit_cmd_selectionChanged();

    void on_Edit_2_cursorPositionChanged();

    void on_Edit_cmd_cursorPositionChanged();

private:
    Ui::MainCmd *ui;

    int last_key = 0; //按下组合键时会被拆成多次调用
    int last_pos = 0;
    int last_block_number = 0;

    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINCMD_H

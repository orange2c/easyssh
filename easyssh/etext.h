#ifndef ETEXT_H
#define ETEXT_H

#include <QObject>
#include <QTextEdit>

class ETEXT : public QObject
{
    Q_OBJECT
public:
    explicit ETEXT( QTextEdit *text_edit, QObject *parent = nullptr);
    ~ETEXT();

    void save_now( QString *text = NULL ); //保存当前文本框内容
    void signal_enable( bool flag ); //设置是否发送文本变动事件信号



protected:
    QTextEdit *Edit;  //指向要操作的文本框
    QTextDocument *Doc; //指向该文本框内的文本
    QString *save_text = NULL; //保存文本框数据，供发生文本改变时比较
    int save_pos = 0;

    bool is_signal_enable = true;

signals:
    void text_change( int delete_count, bool is_backspace, int add_count, QString new_str );
                        //删除计数，是否back（back是左删除，delete键是右删除），新增文本计数，新增文本

public slots:
    void edit_cursor_change(); //记录光标位置，主要用于识别backspace与delete
    void edit_text_change(); //

};

#endif // ETEXT_H

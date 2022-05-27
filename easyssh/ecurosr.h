#ifndef ECUROSR_H
#define ECUROSR_H

#include <QObject>
#include <QTextEdit>
#include <QTextBlock>

class ECUROSR : public QObject
{
    Q_OBJECT
public:
    explicit ECUROSR( QTextEdit *text_edit, QObject *parent = nullptr);

    void move_rel( int pos ); //相对位置移动，如+1 -3

    void signal_enable( bool flag ); //设置是否发送光标变动事件信号


protected:
    QTextEdit *Edit;  //指向一个文本框
    QTextDocument *Doc; //指向文本框内的文本

    bool is_signal_enable = true;

    int save_row[2] ;
    int save_col[2] ;
    int save_pos = 0 ;
    int save_text_count = 0 ; //记录文本大小，如果是因为文本变动导致光标移动，则不发送信号
    int moset_right = 0 ; //记录最近编辑到达的最右一个字符

    inline void save_rc( int row, int column ); //将数据保存，并作左移


signals:
    void cursor_change( int row, int column, int pos ); //光标位置改变，传入相对移动量，如+1 -3

public slots:
    void edit_curosr_change();
};

#endif // ECUROSR_H

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

    bool is_signal_enable = true;

signals:
    void text_del( int count, bool is_backspace ); //count: 删除个数, is_backspace:true表示backspace类型的删除， false表示delete按键引发的类型删除
    void text_add( QString str ); //新增字符串
    void text_add_one();

public slots:
    void edit_text_change(); //
//    void edit_cursor_change(); //主要是记录光标移动来判断是否
};

#endif // ETEXT_H

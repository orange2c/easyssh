#ifndef ETEXT_H
#define ETEXT_H

#include <QObject>

class ETEXT : public QObject
{
    Q_OBJECT
public:
    explicit ETEXT( QTextEdit *text_edit, QObject *parent = nullptr);

    void signal_enable( bool flag ); //设置是否发送文本变动事件信号
    void text_delete( int pos, int count ); //删除指定位置开始的指定数量字符

protected:
    QTextEdit *Edit;  //指向一个文本框
    QString save_text; //保存文本框数据，供发生文本改变时比较

signals:
    void text_del( int count, bool is_backspace ); //count: 删除个数, is_backspace:true表示backspace类型的删除， false表示delete按键引发的类型删除
    void text_add( QString str ); //新增字符串
public slots:
    void edit_text_change(); //
};

#endif // ETEXT_H

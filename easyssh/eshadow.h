#ifndef ESHADOW_H
#define ESHADOW_H

#include <QObject>
#include <QTextCursor>
#include <QTextEdit>

class ESHADOW : public QObject
{
    Q_OBJECT
public:
    explicit ESHADOW( QTextEdit *write, QTextEdit *show, QObject *parent = nullptr);
    ~ESHADOW();
    void updata_show();
    void updata_wirte();

    void shell_listen( QString data );  //接收ssh数据，添加至shadow

    QTextEdit *Edit_original; //用来输出原始数据的窗口
    QTextEdit *Edit_hex; //将收到的ssh数据以hex方式输出的窗口
    QString last_send; //用来记录最近一次发送到ssh的消息，等收到回复时进行比较，以在文本框中删除发送项目


protected:
    QTextEdit *Edit_shadow; //存储中间处理数据
    QTextEdit *Edit_write;  //指向历史记录显示窗口
    QTextEdit *Edit_show;  //指向编辑窗口




signals:

public slots:


};

#endif // ESHADOW_H

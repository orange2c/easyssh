#ifndef SHADOW_H
#define SHADOW_H

#include <QObject>

class Shadow : public QObject
{
    Q_OBJECT
public:
    explicit Shadow(QObject *parent = nullptr);

protected:
    int save_pos = 0; //存储光标位置
    QString save_cmd; //存储当前正在操作的字符串
    QString save_result; //存储上一条结果
    QString save_history; //存储所有历史记录

signals:
    void show( QString &text, bool to_end = false );
    void ssh_write( QString cmd );
public slots:
    void ssh_message( QString text ); //ssh发来新消息
    void change_pos( int pos ); //光标位置改变，传入新光标位置
    void change_text( QString text ); //文本改变，传入新的完整text
};

#endif // SHADOW_H

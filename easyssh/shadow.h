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
    QString save_text; //存储字符

signals:
    void show( QString &text, bool to_end = false );
public slots:
    void ssh_message( QString text ); //ssh发来新消息
    void change_pos( int pos ); //光标位置改变，传入新光标位置
    void change_text( QString &text ); //文本改变，传入新的完整text
};

#endif // SHADOW_H

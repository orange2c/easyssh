#ifndef ECUROSR_H
#define ECUROSR_H

#include <QObject>
#include <QTextEdit>

class ECUROSR : public QObject
{
    Q_OBJECT
public:
    explicit ECUROSR( QTextEdit *text_edit, QObject *parent = nullptr);

    void move_save();  //将光标移动到保存的位置
    void move_end();  //移动到末尾
    void move_rel( int row, int column ); //相对位置移动，如+1 -3

    void set_fence_mode( int mode ); //围栏模式，限制光标只能在特定区域移动，超出强制返回
    void signal_enable( bool flag ); //设置是否启用信号，不启用，则本类依然接收事件进行处理


protected:
    QTextEdit *Edit;  //指向一个文本框
    QTextDocument *Doc; //指向文本框内的文本

    bool is_signal_enable = true;

    int save_row[2] ;
    int save_col[2] ;
    int save_pos = 0 ;

    inline void save_rc( int row, int column ); //将数据保存，并作左移


signals:
    void cursor_change( int row, int column, int pos ); //光标位置改变，传入相对移动量，如+1 -3

public slots:
    void edit_curosr_change();
};

#endif // ECUROSR_H

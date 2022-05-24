#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "pagecmd.h"
#include "ssh.h"

QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QDialog
{
    Q_OBJECT

public:
    login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_released();
    void connect_success();
    void connect_error();


private:
    Ui::login *ui;
    SSH *ssh;
};
#endif // LOGIN_H

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
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

private:
    Ui::login *ui;
    SSH *ssh = new SSH();
};
#endif // LOGIN_H

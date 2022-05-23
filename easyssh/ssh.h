#ifndef SSH_H
#define SSH_H

#include <QObject>
#include <QSharedPointer>
#include "sshconnection.h"
#include "sshremoteprocess.h"
using namespace QSsh;

class SSH : public QObject
{
    Q_OBJECT
public:
    explicit SSH(QObject *parent = nullptr);
    void login( QString Host, QString username, QString password );  //尝试登陆，正确返回0，错误返回小于0
    void write( QString cmd );
protected:
    SshConnectionParameters argment;
    SshConnection *m_connection = NULL ;
    QSharedPointer<QSsh::SshRemoteProcess> m_shell;

    int status_connect = 0; //用于连接时标记是否连接成功，1未返回，0成功，-1失败

signals:
    void connect_success();
    void connect_error();
    void shell_start();
    void shell_output( QString data );
    void shell_err();
    void shell_close();

protected slots:
    void _connect_success();
    void _connect_error();
    void _shell_start();
    void _shell_output();
    void _shell_err();
    void _shell_close();

public slots:
};

#endif // SSH_H

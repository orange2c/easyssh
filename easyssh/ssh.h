#ifndef SSH_H
#define SSH_H

#include <QObject>
#include "sshconnection.h"
using namespace QSsh;

class SSH : public QObject
{
    Q_OBJECT
public:
    explicit SSH(QObject *parent = nullptr);
    void login( QString Host, QString username, QString password );  //尝试登陆，正确返回0，错误返回小于0
    int write();
protected:
    SshConnectionParameters argment;
    SshConnection *m_connection = NULL ;
    QSharedPointer<QSsh::SshRemoteProcess> m_shell;

    int status_connect = 0; //用于连接时标记是否连接成功，1未返回，0成功，-1失败

signals:
    void connect_success();
    void connect_error();
protected slots:
    void _connect_success();
    void _connect_error();

public slots:
};

#endif // SSH_H

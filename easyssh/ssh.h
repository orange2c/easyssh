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
    int login( QString Host, QString username, QString password );  //尝试登陆，正确返回0，错误返回小于0
    int write();
protected:
    SshConnectionParameters argment;
    SshConnection *m_connection = NULL ;
    QSharedPointer<QSsh::SshRemoteProcess> m_shell;

signals:
protected slots:
//    void handleConnected();
    void handleConnectionError();
public slots:
};

#endif // SSH_H

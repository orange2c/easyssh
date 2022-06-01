#include "ssh.h"

SSH::SSH(QObject *parent) : QObject(parent)
{
    argment.port = 22;
    argment.timeout = 1;
    argment.authenticationType =  SshConnectionParameters::AuthenticationTypeTryAllPasswordBasedMethods;
    argment.options &= ~SshIgnoreDefaultProxy;
}
void SSH::login(QString Host, QString username, QString password)
{
    argment.host= Host ;
    argment.userName = username ;
    argment.password = password ;

    if( m_connection != NULL ) delete m_connection;
    m_connection = new SshConnection( argment) ;

    connect(m_connection, SIGNAL( error(QSsh::SshError) ), this, SLOT(_connect_error()));
    connect(m_connection, SIGNAL( connected()), this, SLOT(_connect_success()));

    m_connection->connectToHost();
}
void SSH::write(QString cmd)
{
    m_shell->write(cmd.toUtf8().data());
}

void SSH::_connect_success()
{
    m_shell = m_connection->createRemoteShell();
    connect(m_shell.data(), SIGNAL(started()), SLOT(_shell_start()));
    connect(m_shell.data(), SIGNAL(readyReadStandardOutput()), SLOT(_shell_output()));
//    connect(m_shell.data(), SIGNAL(readyReadStandardError()), SLOT(handleRemoteStderr()));
//    connect(m_shell.data(), SIGNAL(closed(int)), SLOT(handleChannelClosed(int)));

    m_shell->start();


    qDebug("连接成功");

    emit connect_success();
}
void SSH::_connect_error()
{
    qDebug("连接失败");
    emit connect_error();
}
void SSH::_shell_start()
{
    qDebug("start");
    emit shell_start();
}
void SSH::_shell_output()
{
    QString data( m_shell->readAllStandardOutput().data());
    emit shell_output( data );
}
void SSH::_shell_err()
{
    emit shell_err();
}
void SSH::_shell_close()
{
    emit shell_close();
}


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

void SSH::_connect_success()
{
    qDebug("连接成功");
    emit connect_success();
}
void SSH::_connect_error()
{
    qDebug("连接失败");
    emit connect_error();
}

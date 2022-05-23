#include "ssh.h"

SSH::SSH(QObject *parent) : QObject(parent)
{
    argment.port = 22;
    argment.timeout = 1;
    argment.authenticationType =  SshConnectionParameters::AuthenticationTypeTryAllPasswordBasedMethods;
    argment.options &= ~SshIgnoreDefaultProxy;
}
int SSH::login(QString Host, QString username, QString password)
{
    argment.host= Host ;
    argment.userName = username ;
    argment.password = password ;

    if( m_connection != NULL ) delete m_connection;
    m_connection = new SshConnection( argment) ;
    connect(m_connection, SIGNAL(error(QSsh::SshError)), this, SLOT(handleConnectionError()));

    return -2;
}

void SSH::handleConnectionError()
{
    qDebug("connect error");

}

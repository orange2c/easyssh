#ifndef SSH_H
#define SSH_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

#ifdef Q_OS_WIN32
#include <winsock2.h>
#endif

#include <libssh2.h>
#include <QString>
class SSH
{
public:
    SSH();
    ~SSH();

    int connect_ip( QString host ); //与传入ip建立连接，失败则返回负数
    int login( QString user, QString pwd );
    int reconnect();

    void set_login_info( QString host, QString user, QString pwd );
    int login();


    int write( QString cmd );
    QString read();
protected:
     int waitsocket(int socket_fd, LIBSSH2_SESSION *session);

    const char *HOST = "127.0.0.1";
    const char *USER    = "user";
    const char *PASSWORD    = "password";

    unsigned long hostaddr;
    int sock;

    int exitcode;
    char *exitsignal = (char *)"none";
    int bytecount = 0;
    LIBSSH2_CHANNEL *channel;
    LIBSSH2_SESSION *session;

    struct sockaddr_in sin;
    const char *fingerprint;

    int type;
    size_t len;
    LIBSSH2_KNOWNHOSTS *nh;




};

#endif // SSH_H

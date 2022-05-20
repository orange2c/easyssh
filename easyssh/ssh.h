#ifndef SSH_H
#define SSH_H





#ifdef WIN32
#include <winsock2.h>
#else
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
#endif

#include <QString>
class SSH
{
public:
    SSH();
    ~SSH();

    int connect_ip( QString host ); //与传入ip建立连接，失败则返回负数
    int login( QString user, QString pwd );
    int reconnect();

    int write( QString cmd );
    int write2( QString cmd );
    QString read();

    int sock;
protected:
     int waitsocket(int socket_fd, LIBSSH2_SESSION *session);

    const char *HOST = "127.0.0.1";
    const char *USER    = "user";
    const char *PASSWORD    = "password";

    unsigned long hostaddr;


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

#include "ssh.h"

SSH::SSH()
{

}

void SSH::set_login_info(QString host, QString user, QString pwd)
{

    QByteArray *host_str = new QByteArray(host.toLatin1());
    HOST = host_str->data();
    QByteArray *usr_str = new QByteArray(user.toLatin1());
    USER = usr_str->data();
    QByteArray *pword_str = new QByteArray(pwd.toLatin1());
    PASSWORD = pword_str->data();

#ifdef Q_OS_WIN32
    int err;
    err = WSAStartup(MAKEWORD(2, 0), &wsadata);
    if(err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif

    hostaddr = inet_addr(HOST);
    qDebug("HOST=%s",HOST);
}

int SSH::login(void)
{



    /* Ultra basic "connect to port 22 on localhost"
      * Your code is responsible for creating the socket establishing the
      * connection
      */
     sock = socket(AF_INET, SOCK_STREAM, 0);

     sin.sin_family = AF_INET;
     sin.sin_port = htons(22);
     sin.sin_addr.s_addr = hostaddr;
     qDebug("hostaddr = %ld",hostaddr);
     if(connect(sock, (struct sockaddr*)(&sin),
                 sizeof(struct sockaddr_in)) != 0) {
         qDebug("connect !=0");
         return -1;
     }
     qDebug("connect ==0");

     /* Create a session instance */
     session = libssh2_session_init();
     if(!session)
         return -1;
     /* tell libssh2 we want it all done non-blocking */
     libssh2_session_set_blocking(session, 0);

    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */
    int rc;
    while((rc = libssh2_session_handshake(session, sock)) ==
           LIBSSH2_ERROR_EAGAIN);
    if(rc) {
        fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
        return -1;
    }

    nh = libssh2_knownhost_init(session);
    if(!nh) {
        /* eeek, do cleanup here */
        return 2;
    }

    /* read all hosts from here */
    libssh2_knownhost_readfile(nh, "known_hosts",
                               LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    /* store all known hosts to here */
    libssh2_knownhost_writefile(nh, "dumpfile",
                                LIBSSH2_KNOWNHOST_FILE_OPENSSH);

    fingerprint = libssh2_session_hostkey(session, &len, &type);

    if(fingerprint) {
        struct libssh2_knownhost *host;
        int check = libssh2_knownhost_checkp(nh, HOST, 22,
                                             fingerprint, len,
                                             LIBSSH2_KNOWNHOST_TYPE_PLAIN|
                                             LIBSSH2_KNOWNHOST_KEYENC_RAW,
                                             &host);
        fprintf(stderr, "Host check: %d, key: %s\n", check,
                (check <= LIBSSH2_KNOWNHOST_CHECK_MISMATCH)?
                host->key:"<none>");

    }
    else {
        /* eeek, do cleanup here */
        return 3;
    }
    libssh2_knownhost_free(nh);

          while((rc = libssh2_userauth_password(session, USER, PASSWORD)) ==
                 LIBSSH2_ERROR_EAGAIN);
          if(rc) {
              fprintf(stderr, "Authentication by password failed.\n");
          }


    return 0;
}
int SSH::waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&fd);

    FD_SET(socket_fd, &fd);

    /* now make sure we wait in the correct direction */
    dir = libssh2_session_block_directions(session);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;

    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;

    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

    return rc;
}

int SSH::write(QString cmd)
{
    int rc;
    char *cmdline = QByteArray(cmd.toLatin1()).data();

    /* Exec non-blocking on the remove host */
    while((channel = libssh2_channel_open_session(session)) == NULL &&
          libssh2_session_last_error(session, NULL, NULL, 0) ==
          LIBSSH2_ERROR_EAGAIN) {
        waitsocket(sock, session);
    }
    if(channel == NULL) {
        fprintf(stderr, "Error\n");
        exit(1);
    }

    while((rc = libssh2_channel_exec(channel, cmdline)) ==
           LIBSSH2_ERROR_EAGAIN) {
        waitsocket(sock, session);
    }
    if(rc != 0) {
        fprintf(stderr, "Error\n");
        exit(1);
    }
    return 0;

}
QString SSH::read()
{
    int rc;
    QString re_value;
    while(1)
    {
        char buffer[0x4000];
           /* loop until we block */
        do {
            rc = libssh2_channel_read(channel, buffer, sizeof(buffer) );
            if(rc > 0) {
                   int i;
                   bytecount += rc;
                   fprintf(stderr, "We read:\n");
                   for(i = 0; i < rc; ++i)
                   {
                       re_value.append(buffer[i]);
                       fputc(buffer[i], stderr);

                   }
                   fprintf(stderr, "\n");
               }
               else {
                   if(rc != LIBSSH2_ERROR_EAGAIN)
                       /* no need to output this for the EAGAIN case */
                       fprintf(stderr, "libssh2_channel_read returned %d\n", rc);
               }
           }
           while(rc > 0);

           /* this is due to blocking that would occur otherwise so we loop on
              this condition */
           if(rc == LIBSSH2_ERROR_EAGAIN) {
               waitsocket(sock, session);
           }
           else
               break;
       }
       exitcode = 127;
       while((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN)
           waitsocket(sock, session);

       if(rc == 0) {
           exitcode = libssh2_channel_get_exit_status(channel);
           libssh2_channel_get_exit_signal(channel, &exitsignal,
                                           NULL, NULL, NULL, NULL, NULL);
       }

       if(exitsignal)
           fprintf(stderr, "\nGot signal: %s\n", exitsignal);
       else
           fprintf(stderr, "\nEXIT: %d bytecount: %d\n", exitcode, bytecount);

    return re_value;

}

void SSH::shutdown()
{
    libssh2_channel_free(channel);
    channel = NULL;
}

#ifndef __LITENET_H__
#define __LITENET_H__

#define REDIS_BINDADDR_MAX 16

typedef struct netServer{
    pid_t pid;
    aeEventLoop *el;

    int port;                   /* TCP listening port */
    int tcp_backlog;            /* TCP listen() backlog */
    char *bindaddr[REDIS_BINDADDR_MAX]; /* Addresses we should bind to */
    int bindaddr_count;         /* Number of addresses in server.bindaddr[] */
    char *unixsocket;           /* UNIX socket path */
    mode_t unixsocketperm;      /* UNIX socket permission */
    int ipfd[REDIS_BINDADDR_MAX]; /* TCP socket file descriptors */
    int ipfd_count;             /* Used slots in ipfd[] */
    int sofd;                   /* Unix socket file descriptor */
    int tcpkeepalive;               /* Set SO_KEEPALIVE if non-zero. */

    list *clients;              /* List of active clients */
    list *clients_to_close;     /* Clients to close asynchronously */
    uint64_t next_client_id;    /* Next client unique ID. Incremental. */
}netServer;

typedef struct netClient {
    uint64_t id;            /* Client incremental unique ID. */
    int fd;
    sds querybuf;
    size_t querybuf_peak;   /* Recent (100ms or more) peak of querybuf size */
    
    /* Response buffer */
    int bufpos;
    char buf[REDIS_REPLY_CHUNK_BYTES];
}netClient;

#endif
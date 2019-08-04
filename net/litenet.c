#include "litenet.h"
#include "anet.h"


netServer server;

void initServer(void) {
    int j;
    server.pid = getpid();
    server.clients = listCreate();
    server.clients_to_close = listCreate();
}

netClient *createClient(int fd) {
    netClient *c = malloc(sizeof(netClient));

    if (fd != -1) {
        anetNonBlock(NULL,fd);
        anetEnableTcpNoDelay(NULL,fd);
        if (server.tcpkeepalive)
            anetKeepAlive(NULL,fd,server.tcpkeepalive);
    }
    else{
        free(c);
        c = NULL;
        return c;
    }

    c->id = server.next_client_id++;
    c->fd = fd;
    c->bufpos = 0;
    c->querybuf = sdsempty();
    c->querybuf_peak = 0;
    if (fd != -1) listAddNodeTail(server.clients,c);
    return c;
}
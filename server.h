#ifndef _SERVER_H
#define _SERVER_H

#include    <stdint.h>

#include    "ircio.h"

#ifndef NULL
#   define NULL (void *)0
#endif

#define SOCK            send_fifo->sock = srv->sock
#define IRCCONNECT()    {\
    SOCK = connection (srv->address, srv->port);\
    sprintf (senb, "NICK %s\r\n", srv->nickname);\
    queue_push (send_fifo, senb);\
    sprintf (senb, "USER %s 0 * :%s\r\n", srv->username, srv->realname);\
    queue_push (send_fifo, senb);\
}
#define CHAN_JOIN       for (i = 0; i < srv->nchan; i++)\
    {\
        sprintf (senb, "JOIN %s\r\n", srv->chans[i]);\
        queue_push (send_fifo, senb);\
    }\

typedef uint16_t            SIGNAL;
typedef struct names        names;
typedef struct ircserver    ircserver;

struct ircserver
{
    char        *   nickname;
    char        *   username;
    char        *   realname;
    char        *   address;
    int             port;
    char            ssl;
    char        **  chans;
    int             nchan;
    int             sock;
    ircserver   *   next;
};

struct names
{
    char * nickname;
    char * username;
    char * realname;
};

#include    "plugins.h"

extern int servlen (ircserver *);
extern ircserver * ircserver_init (names *);
extern void * new_server (void *);

#endif

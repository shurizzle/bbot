#ifndef _SERVER_H
#define _SERVER_H

#include    "ircio.h"

#ifndef NULL
#   define NULL (void *)0
#endif

typedef struct names names;
typedef struct ircserver ircserver;

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

extern int servlen (ircserver *);
extern ircserver * ircserver_init (names *);
extern int start (ircserver *, queue *, message *);
extern void * new_server (void *);

#endif

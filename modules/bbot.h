#include    <pthread.h>

typedef struct ircserver    ircserver;
typedef struct tosend       tosend;
typedef struct queue        queue;
typedef struct function     function;

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

struct tosend
{
    char    *   text;
    tosend  *   next;
};

struct queue
{
    tosend      *   msg;
    tosend      *   last;
    pthread_mutex_t mutex;
    int             sock;
};

#define BOTFUNC(name)   int name (ircserver * srv, queue * fifo)

struct function
{
    int signal;
    char * command;
    int (*func) (ircserver *, queue *);
    char * doc;
};

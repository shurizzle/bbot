#ifndef IRCIO_H
#define IRCIO_H

#include    <pthread.h>

#include    <ircparser.h>

typedef struct queue queue;

#include    <server.h>

typedef struct tosend tosend;

enum
{
    BUFFERSIZE = 513,
    NOFLOODTIME = 700000
};

struct tosend
{
    char * text;
    tosend * next;
};

struct queue
{
    tosend * msg;
    tosend * last;
    pthread_mutex_t mutex;
    int sock;
};

extern char * init_buffer (void);
extern queue * queue_init (void);
extern int connection (char *, int);
extern void _send (int, char *);
extern void _close (int);
extern char * read_stream (int);
extern void * msgsender (void *);
extern void queue_push (queue *, char *);
extern char * queue_pop (queue *);

#endif

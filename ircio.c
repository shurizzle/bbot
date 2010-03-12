#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ircio.h"
#include "errors.h"
#include "ircparser.h"
#include "debug.h"
#include "server.h"
#include "string.h"

#ifndef h_addr
#   define  h_addr  h_addr_list[0]
#endif

extern int usleep (__useconds_t __useconds);

char *
init_buffer ()
{
    return (char *) calloc (BUFFERSIZE, sizeof (char));
}

int
connection (char * server, int port)
{
        struct sockaddr_in info;
        struct hostent * host = gethostbyname (server);
        int sock;

        info.sin_family = AF_INET;
        info.sin_port = htons (port);
        info.sin_addr = * ((struct in_addr *) host->h_addr);
        memset (info.sin_zero, 0, sizeof (info.sin_zero));

        if ((sock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
            die ("Socket creation failed", SOCK_ERROR);

        if (connect (sock, (struct sockaddr *) &info, sizeof (info)) < 0)
            die ("Connection failed", CONNECT_FAIL);

        puts (":: Connection established ::");

        return sock;
}

void
_send (int sock, char * message)
{
    write (sock, message, strlen (message));
    printf ("<< %.*s\n", strlen (message) - 2, message);
}

void
_close (int sock)
{       
    close (sock);
    puts (":: Socket closed ::");
}

char *
read_stream (int sock)
{
    int len = 0;
    char * recv = init_buffer ();

    while (strstr (recv, "\r\n") == NULL)
        read (sock, recv + len++, sizeof (char));

    * (strstr (recv, "\r\n")) = '\0';
    recv = (char *) realloc (recv, (strlen (recv) + 1) * sizeof (char));
    printf (">> %s\n", recv);

    return recv;
}

void *
msgsender (void * _send_fifo)
{
    char * send_buff;
    queue * send_fifo = (queue *) _send_fifo;
    while (1)
    {
        pthread_mutex_lock (&send_fifo->mutex);
        if (send_fifo->msg != NULL)
        {
            send_buff = queue_pop (send_fifo);
            _send (send_fifo->sock, send_buff);
            free (send_buff);
        }
        pthread_mutex_unlock (&send_fifo->mutex);
        usleep (700000);
    }
}

void
queue_push (queue * send_fifo,
            char * msg)
{
    tosend * newsend = (tosend *) malloc (sizeof (tosend));
    newsend->text = strdup (msg);
    newsend->next = NULL;
    pthread_mutex_lock (&send_fifo->mutex);
    if (send_fifo->msg == NULL)
    {
        send_fifo->msg = newsend;
        send_fifo->last = NULL;
    }
    if (send_fifo->last != NULL)
        send_fifo->last->next = newsend;
    send_fifo->last = newsend;
    pthread_mutex_unlock (&send_fifo->mutex);
}

char *
queue_pop (queue * send_fifo)
{
    char * res;
    tosend * tmp;
    res = strdup (send_fifo->msg->text);
    tmp = send_fifo->msg->next;
    free (send_fifo->msg);
    send_fifo->msg = tmp;
    return res;
}

queue *
queue_init (void)
{
    queue * res = (queue *) malloc (sizeof (queue));
    res->msg = res->last = NULL;
    pthread_mutex_init (&res->mutex, NULL);
    res->sock = -1;
    return res;
}

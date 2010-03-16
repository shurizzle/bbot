#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <unistd.h>

#define __USE_UNIX98
#include    <signal.h>

#include    "ircio.h"
#include    "server.h"
#include    "signals.h"

int
servlen (ircserver * srv)
{
    int i = 1;
    ircserver * tmp = srv;
    if (srv == NULL)
        return 0;

    while (tmp->next != NULL)
    {
        i++;
        tmp = tmp->next;
    }
    return i;
}

ircserver *
ircserver_init (names * nm)
{
    ircserver * srv = (ircserver *) malloc (sizeof (ircserver));
    srv->nickname = nm->nickname;
    srv->username = nm->username;
    srv->realname = nm->realname;
    srv->address = (char *) 0;
    srv->chans = (char **) 0;
    srv->next = (ircserver *) 0;
    srv->port = srv->nchan = srv->nchan = srv->ssl = 0;

    return srv;
}

void *
new_server (void * data)
{
    ircserver * srv = (ircserver *) data;
    int i;
    char * senb = init_buffer (), * recb = init_buffer ();
    message * parsed_msg;
    pthread_t message_sender;
    queue * send_fifo = queue_init ();
    SIGNAL signal = 0;

    IRCCONNECT ();
    pthread_create (&message_sender, NULL, msgsender, (void *) send_fifo);
    while (!((SOCK) < 0))
    {
        recb = read_stream (srv->sock);
        parsed_msg = parse_raw (recb);
        signal = get_signal (parsed_msg, srv);

        if (signal == ERROR)
            SOCK = -1;

        if (signal == PING)
        {
            sprintf (senb, "PONG %s\r\n", parsed_msg->parameters[0]);
            queue_push (send_fifo, senb);
            continue;
        }

        if (signal == ERROR)
            IRCCONNECT ();

        if (signal == MODE && i != srv->nchan)
            CHAN_JOIN;

        if (signal == JOIN_OWN)
        {
            sprintf (senb, "PRIVMSG %s :Bronsa gay\r\n", parsed_msg->parameters[0] + 1);
            queue_push (send_fifo, senb);
            sprintf (senb, "PRIVMSG %s :DIO PORCO\r\n", parsed_msg->parameters[0] + 1);
            queue_push (send_fifo, senb);
            sprintf (senb, "PRIVMSG %s :I <3 yawn\r\n", parsed_msg->parameters[0] + 1);
            queue_push (send_fifo, senb);
            sprintf (senb, "PRIVMSG %s :Malex vs IRC, chi vincerà?\r\n", parsed_msg->parameters[0] + 1);
            queue_push (send_fifo, senb);
        }

        free_message (parsed_msg);
    }
    
    free (recb);
    free (senb);
    pthread_kill (message_sender, SIGKILL);
    _close (srv->sock);

    return NULL;
}

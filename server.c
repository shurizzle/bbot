#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <unistd.h>

#include    "ircio.h"
#include    "server.h"

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

int
start (ircserver * srv, queue * send_fifo, message * msg)
{
    char * buffer = init_buffer ();
    int cstart;
    if (!strncmp (msg->parameters[msg->parno - 1], ":*** Looking up your hostname...", 31))
    {
        cstart = 1;
        sleep (1);
    }
    else if (!strncmp (msg->parameters[msg->parno - 1], ":*** Found your hostname (cached)", 32))
    {
        sprintf (buffer, "NICK %s\r\n", srv->nickname);
        queue_push (send_fifo, buffer);
        sprintf (buffer, "USER %s 0 * :%s\r\n", srv->username, srv->realname);
        queue_push (send_fifo, buffer);
        cstart = 1;
    }
    else
        cstart = 0;

    free (buffer);

    return cstart;
}

void *
new_server (void * data)
{
    ircserver * srv = (ircserver *) data;
    int i;
    int cstart = 1;
    char * senb = init_buffer (), * recb = init_buffer ();
    message * parsed_msg;
    pthread_t message_sender;
    queue * send_fifo = queue_init ();

    puts ("Connection");
    send_fifo->sock = srv->sock = connection (srv->address, srv->port);
    pthread_create (&message_sender, NULL, msgsender, (void *) send_fifo);
    while (1)
    {
        recb = read_stream (srv->sock);
        if (!strncmp ("PING", recb, 4))
        {
            sprintf (senb, "PO%s\r\n", recb + 2);
            queue_push (send_fifo, senb);
            continue;
        }

        parsed_msg = parse_raw (recb);

        if (cstart)
        {
            parsed_msg = parse_raw (recb);
            cstart = start (srv, send_fifo, parsed_msg);
            free_message (parsed_msg);
            continue;
        }

        if (!strcmp (parsed_msg->command, "MODE") && !strcmp (parsed_msg->parameters[0], srv->nickname)
            && parsed_msg->source->server != NULL && !strcmp (parsed_msg->source->server, srv->nickname))
        {
            for (i = 0; i < srv->nchan; i++)
            {
                sprintf (senb, "JOIN %s\r\n", srv->chans[i]);
                queue_push (send_fifo, senb);
                sprintf (senb, "PRIVMSG %s :Bronsa gay\r\n", srv->chans[i]);
                queue_push (send_fifo, senb);
                queue_push (send_fifo, senb);
                queue_push (send_fifo, senb);
                queue_push (send_fifo, senb);
                queue_push (send_fifo, senb);
            }
        }
        free_message (parsed_msg);
        free (recb);
    }
    _close (srv->sock);
}

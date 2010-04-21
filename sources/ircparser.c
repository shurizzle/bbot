#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <ircparser.h>
#include <errors.h>
#include <ircio.h>
#include <debug.h>

message *
new_message ()
{
    message * new;
    if ((new = (message *) malloc (sizeof (message))) == NULL)
       die ("Memory allocation failed", ALLOC_FAIL);
    if ((new->source = (sender *) malloc (sizeof (sender))) == NULL)
       die ("Memory allocation failed", ALLOC_FAIL);

    new->source->nick = new->source->server = new->source->user = new->source->host = new->command = (char *) NULL;
    new->parameters = (char **) NULL;
    new->parno = new->_paralloc = 0;

    return new;
}

char **
addparam (message * msg)
{
    msg->parno += 1;

    if (msg->parno == 1)
    {
        msg->parameters = (char **) calloc (PAR_BLOCK, sizeof (char *));
        msg->_paralloc = PAR_BLOCK;
    }
    else
        if (msg->parno > msg->_paralloc)
            msg->parameters = (char **) realloc (msg->parameters, (msg->_paralloc += PAR_BLOCK) * sizeof (char *));

    return &msg->parameters[msg->parno - 1];
}

void
free_message (message * msg)
{
    int i;

    free (msg->source->nick);
    free (msg->source->server);
    free (msg->source->user);
    free (msg->source->host);
    free (msg->command);

    for (i = 0; i < msg->parno; i++)
        free (msg->parameters[i]);

    free (msg->parameters);
    free (msg);
}

void
AES (char ** dest,
     char * source,
     int * offset,
     int * pos)
{
    *dest = (char *) calloc (((*pos - *offset) + 3), sizeof (char));
    strncat (*dest, source + *offset, *pos - *offset + 1);
    *pos += 2;
    *offset = *pos;
}

message *
parse_raw (char * incoming)
{
    int pos = 0, offset = 0, incominglen = strlen (incoming);
    message * ret = new_message ();

    if (incoming[0] == ':')
    {
        offset = 1;
        for (pos = 1; incoming[pos + 1] != ' ' && incoming[pos + 1] != '!'; pos++);
        if (incoming[pos + 1] == ' ')
            AES (&ret->source->server, incoming, &offset, &pos);
        else
            AES (&ret->source->nick, incoming, &offset, &pos);

        if (ret->source->nick != NULL)
        {
            for (; incoming[pos + 1] != '@'; pos++);
            AES (&ret->source->user, incoming, &offset, &pos);

            for (; incoming[pos + 1] != ' '; pos++);
            AES (&ret->source->host, incoming, &offset, &pos);
        }
    }

    for (pos -= 1; incoming[pos + 1] != ' '; pos++);
    AES (&ret->command, incoming, &offset, &pos);

    while (incoming[pos] != ':' && pos < incominglen)
    {
        for (; incoming[pos + 1] != ' ' && (pos + 1) < incominglen; pos++);
        AES (addparam (ret), incoming, &offset, &pos);
    }

    if ((pos + 1) < incominglen)
    {
        pos = incominglen - 1;
        AES (addparam (ret), incoming, &offset, &pos);
    }

    ret->parameters = (char **) realloc (ret->parameters, (ret->_paralloc = ret->parno) * sizeof (char *));

    return ret;
}

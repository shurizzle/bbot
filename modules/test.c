#include    <stdio.h>
#include    "bbot.h"
#include    "signals.h"

#ifndef NULL
#   define  NULL    (void *) 0
#endif

BOTFUNC (ping)
{
    char * buffer = init_buffer ();

    sprintf (buffer, "PONG %s\r\n", msg->parameters[0]);
    queue_push (send_fifo, buffer);
    free (buffer);

    return 1;
}

function functions[] = {
        {PING, NULL, ping, "pong at ping"},
    };

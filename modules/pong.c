#include    <stdio.h>
#include    <stdlib.h>
#include    "bbot.h"

BOTFUNC (ping)
{
    char * buffer = init_buffer ();

    sprintf (buffer, "PONG %s\r\n", msg->parameters[0]);
    queue_push (fifo, buffer);
    free (buffer);

    return 1;
}

function functions[] = {
        {"PING", ping, PING, NULL, "pong at ping"}
    };

char * library_path[] = {0};

char * libraries[] = {0};

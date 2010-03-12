#include <stdio.h>

#include "debug.h"

void
print_message (message * parsed_msg)
{
    int i;
    if (parsed_msg->source->nick != NULL)
        printf ("USER: %s ! %s @ %s\n", parsed_msg->source->nick, parsed_msg->source->user, parsed_msg->source->host);
    else if (parsed_msg->source->server != NULL)
        printf ("SERVER: %s\n", parsed_msg->source->server);
    printf ("COMMAND: %s\n", parsed_msg->command);
    puts ("PARAMETERS:");
    for (i = 0; i < parsed_msg->parno; i++)
        printf ("  %d: %s\n", i + 1, parsed_msg->parameters[i]);
}

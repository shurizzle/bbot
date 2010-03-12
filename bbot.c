#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <pthread.h>

#include    "ircio.h"
#include    "errors.h"
#include    "ircparser.h"
#include    "debug.h"
#include    "server.h"
#include    "bbot.h"
#include    "string.h"
#include    "confparser.h"

char * owner = "shura";

int
main ()
{
    int i;
    pthread_t * tid_srvs;
    names * nm;
    ircserver * srvs, * srv;

    nm = load_names ("configs.xml");
    srvs = load_servers ("configs.xml", nm);
    srv = srvs;

    tid_srvs = (pthread_t *) calloc (servlen (srvs) , sizeof (pthread_t));
    for (i = 0; i < servlen (srvs); i++)
    {
        pthread_create (&tid_srvs[i], NULL, new_server, (void *) srv);
        srv = srv->next;
    }
    for (i = 0; i < servlen (srvs); i++)
        pthread_join (tid_srvs[i], NULL);

    return 0;
}

#include    <stdlib.h>
#include    <string.h>

#include    <signals.h>

#define cmd msg->command
#define prm msg->parameters

int
get_signal (message * msg, ircserver * srv)
{
    if (IS_NUM (cmd[0]) && IS_NUM (cmd[1]) && IS_NUM (cmd[2]))
        return atoi (cmd);
    SET_OWN_SIGNAL (JOIN_OWN, msg->source->nick);
    SET_OWN_SIGNAL (KILL_OWN, prm[0]);
    SET_OWN_SIGNAL (KICK_OWN, prm[1]);
    SET_SIGNAL (PING);
    SET_SIGNAL (JOIN);
    SET_SIGNAL (KILL);
    SET_SIGNAL (MODE);
    SET_SIGNAL (KICK);
    SET_SIGNAL (ERROR);
    SET_SIGNAL (NOTICE);
    SET_OTH_SIGNAL (QUERY, IS ("PRIVMSG") && AMI (prm[0]));
    SET_OTH_SIGNAL (MESSAGE, IS ("PRIVMSG"));
    return 0;
}

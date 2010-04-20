#ifndef _BBOT_H
#define _BBOT_H
#define BOTFUNC(name)   int name (ircserver * srv, queue * fifo, message * msg)
#include    "../ircio.h"
#include    "../errors.h"
#include    "../ircparser.h"
#include    "../debug.h"
#include    "../server.h"
#include    "../bbot.h"
#include    "../string.h"
#include    "../confparser.h"
#include    "../plugins.h"
#include    "../signals.h"
#endif

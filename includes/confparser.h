#ifndef _CONF_PARSER_H
#define _CONF_PARSER_H

#include    <libxml/tree.h>
#include    <libxml/parser.h>

#include    <server.h>

extern ircserver * get_info_server (xmlNodePtr, names *);
extern names * load_names (const char *);
extern ircserver * load_servers (const char *, names *);
/*extern plugin * load_plugins (const char *);*/

#endif

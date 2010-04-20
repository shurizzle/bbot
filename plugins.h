#ifndef _PLUGINS_H
#define _PLUGINS_H

#include    <string.h>

#include    "server.h"
#include    "ircio.h"
#include    "ircparser.h"

typedef struct MODULE   MODULE;
typedef struct PLUGIN   PLUGIN;
typedef struct function function;

struct MODULE
{
    char    *   name;
    void    *   handle;
    MODULE  *   next;
};

struct PLUGIN
{
    int         signal;
    char    *   name;
    char    *   module;
    int    (*   exec)   (ircserver *, queue *, message *);
    char    *   doc;
    PLUGIN  *   next;
};

struct function
{
    char    *   name;
    int    (*   func) (ircserver *, queue *, message *);
    int         signal;
    char    *   command;
    char    *   doc;
};

extern  MODULE  *   modules;
extern  PLUGIN  *   plugins;

extern  MODULE  *   module_init (void);
extern  void        module_append (MODULE *);
extern  void        module_free (MODULE *);
extern  void        module_delete (MODULE *);
extern  void        module_delete_by_name (char *);
extern  void        plugin_delete (PLUGIN *);
extern  void        plugin_delete_by_name (char *);
extern  void        plugin_delete_by_module (char *);
extern  void        load_module (char *);
extern  void        load_plugins (char *, function []);
extern  void        load_lib (char * lib);

#endif

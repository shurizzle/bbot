#ifndef _PLUGINS_H
#define _PLUGINS_H

#include    "server.h"
#include    "ircio.h"
#include    "string.h"

typedef struct MODULE   MODULE;
typedef struct PLUGIN   PLUGIN;
typedef struct function function;
typedef function * (* init_func) (void);

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
    int    (*   exec)   (ircserver *, queue *);
    char    *   doc;
    PLUGIN  *   next;
};

struct function
{
    int         signal;
    char    *   command;
    int    (*   func) (ircserver *, queue *);
    char    *   doc;
};

static  MODULE  *   modules = NULL;
static  PLUGIN  *   plugins = NULL;

extern  MODULE  *   module_init (void);
extern  void        module_append (MODULE *);
extern  void        module_free (MODULE *);
extern  void        module_delete (MODULE *);
extern  void        load_module (char *);
extern  void        load_plugins (function []);

#endif

#ifndef _PLUGINS_H
#define _PLUGINS_H

typedef struct MODULE MODULE;
typedef struct PLUGIN PLUGIN;

struct MODULE
{
    char * name;
    void * handle;
};

struct PLUGIN
{
    char * name;
    char * module;
    int (* exec)(int, char **);
    char * doc;
};

#define MD_BLOCK    10

extern  PLUGIN * load_module (char *);
extern  void load_plugins (PLUGIN *);

#endif

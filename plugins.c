#include    <stdlib.h>
#include    <stdio.h>
#include    <dlfcn.h>

#include    "plugins.h"
#include    "string.h"

MODULE * modules = NULL;
PLUGIN * plugins = NULL;

MODULE *
module_init (void)
{
    MODULE * newmodule = (MODULE *) calloc (1, sizeof (MODULE));
    newmodule->name = newmodule->handle = newmodule->next = NULL;
    return newmodule;
}

PLUGIN *
plugin_init (void)
{
    PLUGIN * newplugin = (PLUGIN *) calloc (1, sizeof (PLUGIN));
    newplugin->name = newplugin->module = newplugin->doc = NULL;
    newplugin->signal = 0;
    newplugin->exec = NULL;
    newplugin->next = NULL;
    return newplugin;
}

void
module_append (MODULE * to_append)
{
    MODULE * tmp = modules;
    if (modules == NULL)
    {
        modules = to_append;
        return;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = to_append;
}

void
plugin_append (PLUGIN * to_append)
{
    PLUGIN * tmp = plugins;
    if (plugins == NULL)
    {
        plugins = to_append;
        return;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = to_append;
}

void
module_free (MODULE * to_free)
{
    free (to_free->name);
    free (to_free->handle);
    free (to_free);
}

void
plugin_free (PLUGIN * to_free)
{
    free (to_free->name);
    free (to_free->module);
    free (to_free->doc);
    free (to_free);
}

void
module_delete_by_name (char * name)
{
    MODULE * tmp = modules, * todel = NULL;

    if (!strcmp (tmp->name, name))
    {
        modules = modules->next;
        module_free (tmp);
        return;
    }

    while (strcmp (tmp->next->name, name))
        tmp = tmp->next;

    todel = tmp->next;
    tmp->next = todel->next;
    plugin_delete_by_module (todel->name);
    module_free (todel);
}

void
module_delete (MODULE * to_del)
{
    MODULE * tmp = modules;

    if (tmp == to_del)
    {
        modules = modules->next;
        module_free (tmp);
        return;
    }

    while (tmp->next != to_del)
        tmp = tmp->next;

    tmp->next = to_del->next;
    plugin_delete_by_module (to_del->name);
    module_free (to_del);
}

void
plugin_delete_by_module (char * module)
{
    PLUGIN * tmp = plugins, * todel = NULL;

    if (!strcmp (tmp->module, module))
    {
        plugins = plugins->next;
        plugin_free (tmp);
        return;
    }

    while (strcmp (tmp->next->module, module))
        tmp = tmp->next;

    todel = tmp->next;
    tmp->next = todel->next;
    plugin_free (todel);
}

void
plugin_delete_by_name (char * name)
{
    PLUGIN * tmp = plugins, * todel = NULL;

    if (!strcmp (tmp->name, name))
    {
        plugins = plugins->next;
        plugin_free (tmp);
        return;
    }

    while (strcmp (tmp->next->name, name))
        tmp = tmp->next;

    todel = tmp->next;
    tmp->next = todel->next;
    plugin_free (todel);
}

void
plugin_delete (PLUGIN * to_del)
{
    PLUGIN * tmp = plugins;

    if (tmp == to_del)
    {
        plugins = plugins->next;
        plugin_free (tmp);
        return;
    }

    while (tmp->next != to_del)
        tmp = tmp->next;

    tmp->next = to_del->next;
    plugin_free (to_del);
}

void
load_module (char * file)
{
    MODULE * module = module_init ();
    char * error, * x, i;
    function * functions;

    for (i = 0, x = file; *x; x++)
        if (*x == '/')
            i = x - file + 1;

    module->name = strdup (file + i);

    module->handle = dlopen (file, RTLD_LAZY);
    if (!module->handle)
    {
        fprintf (stderr, "Couldn't load %s: %s\n", file, dlerror ());
        return;
    }

    fprintf (stderr, "Loaded module %s\n", module->name);

    dlerror ();
    functions = (function *) dlsym (module->handle, "functions");
    if ((error = dlerror ()))
    {
        fprintf (stderr, "Invalid module, couldn't found initialization\n\t%s\n", error);
        return;
    }
    
    load_plugins (module->name, functions);
    module_append (module);
}

void
load_plugins (char * modname, function * fcns)
{
    int i;
    PLUGIN * new;
    for (i = 0; fcns[i].name != NULL; i++)
    {
        new = plugin_init ();
        new->signal = fcns[i].signal;
        new->name = strdup (fcns[i].name);
        new->module = modname;
        new->exec = fcns[i].func;
        new->doc = strdup (fcns[i].doc);
        plugin_append (new);
    }
}

void
load_lib (char * lib)
{
    void * handle = dlopen (lib, RTLD_GLOBAL|RTLD_NOW);
    if (!handle)
    {
        fprintf (stderr, "Couldn't load %s: %s\n", lib, dlerror ());
        return;
    }
    fprintf (stderr, "Loaded module %s\n", lib);
}

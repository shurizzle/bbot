#include    <stdlib.h>
#include    <stdio.h>
#include    <dlfcn.h>

#include    "plugins.h"

MODULE *
module_init (void)
{
    MODULE * newmodule = (MODULE *) calloc (1, sizeof (MODULE));
    newmodule->name = newmodule->handle = newmodule->next =NULL;
    return newmodule;
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
module_free (MODULE * to_free)
{
    free (to_free->name);
    free (to_free->handle);
    free (to_free);
}

void
module_delete (MODULE * to_del)
{
    MODULE * tmp = modules;

    if (tmp == to_del)
    {
        modules = modules->next;
        module_free (tmp);
    }

    while (tmp->next != to_del)
        tmp = tmp->next;

    tmp->next = to_del->next;
    module_free (to_del);
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
    
    load_plugins ((function *) dlsym (module->handle, "functions"));
    module_append (module);
}

void
load_plugins (function fcns[])
{
}

#include <string.h>
#include <stdlib.h>

char *
strdup (const char * str)
{
    char * dup = (char *) calloc (strlen (str) + 1, sizeof (char));
    strcpy (dup, str);
    return dup;
}

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

void
die (const char * message,
     int errno)
{
    perror (message);
    exit (errno);
}

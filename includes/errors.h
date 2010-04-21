#ifndef _ERRORS_H
#define _ERRORS_H

enum
{
    NORMAL_EXIT,
    SOCK_ERROR,
    CONNECT_FAIL,
    ALLOC_FAIL
};

extern void die (const char * message, int ex_code);

#endif

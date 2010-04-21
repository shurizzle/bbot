#ifndef _PARSER_H
#define _PARSER_H   1
#define PAR_BLOCK   10

typedef struct sender sender;
typedef struct message message;

struct sender
{
    char * nick;
    char * server;
    char * user;
    char * host;
};

struct message
{
    sender * source;
    char * command;
    char ** parameters;
    int parno;
    int _paralloc;
};

extern message * new_message ();
extern char ** addparam (message *);
extern void free_message (message *);
extern void AES (char **, char *, int *, int *);
extern message * parse_raw (char *);

#endif

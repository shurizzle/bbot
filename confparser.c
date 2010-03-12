#include    <stdlib.h>
#include    <stdio.h>
#include    <string.h>
#include    <libxml/parser.h>
#include    <libxml/tree.h>

#include    "confparser.h"
#include    "server.h"

#define     CH_BLOCK    10

int
xstrcmp (char * penis,
         char * lopo)
{
    return strcmp ((char *) penis, (char *) lopo);
}

names *
load_names (const char * file)
{
    xmlDocPtr doc = NULL;
    xmlNodePtr node = NULL;
    names * nm = (names *) malloc (sizeof (names));

    nm->nickname = nm->username = nm->realname = NULL;

    LIBXML_TEST_VERSION

    if ((doc = xmlReadFile (file, NULL, 0)) == NULL)
        fprintf (stderr, "error: couldn't parse file %s\n", file);

    node = xmlDocGetRootElement (doc);

    if (xmlStrcmp (node->name, (xmlChar *) "configs"))
    {
        fprintf (stderr, "Error when loading configuration\n");
    }

    node = node->children;

    while (node != NULL)
    {
        if (node->type == XML_ELEMENT_NODE && node->children->type == XML_TEXT_NODE)
        {
            if (!xmlStrcmp (node->name, (xmlChar *) "nickname"))
                nm->nickname = (char *) xmlStrdup (node->children->content);
            else if (!xmlStrcmp (node->name, (xmlChar *) "username"))
                nm->username = (char *) xmlStrdup (node->children->content);
            else if (!xmlStrcmp (node->name, (xmlChar *) "realname"))
                nm->realname = (char *) xmlStrdup (node->children->content);
        }

        node = node->next;
    }

    xmlFreeDoc (doc);
    xmlFreeNode (node);
    xmlCleanupParser();

    return nm;
}

ircserver *
get_info_server (xmlNodePtr node, names * nm)
{
    ircserver * srv;
    
    srv = ircserver_init (nm);

    while (node)
    {
        if (node->type == XML_ELEMENT_NODE && node->children->type == XML_TEXT_NODE)
        {
            if (!xmlStrcmp (node->name, (xmlChar *) "nickname"))
                srv->nickname = (char *) xmlStrdup (node->children->content);
            else if (!xmlStrcmp (node->name, (xmlChar *) "username"))
                srv->username = (char *) xmlStrdup (node->children->content);
            else if (!xmlStrcmp (node->name, (xmlChar *) "realname"))
                srv->realname = (char *) xmlStrdup (node->children->content);
            else if (!xmlStrcmp (node->name, (xmlChar *) "address"))
                srv->address = (char *) xmlStrdup (node->children->content);
            else if (!xmlStrcmp (node->name, (xmlChar *) "port"))
                srv->port = atoi ((char*) node->children->content);
            else if (!xmlStrcmp (node->name, (xmlChar *) "ssl"))
            {
                if (!xmlStrcmp (node->children->content, (xmlChar *) "yes"))
                    srv->ssl = 1;
            }
            else if (!xmlStrcmp (node->name, (xmlChar *) "channels"))
            {
                node = node->children;
                do
                {
                    if (node->type == XML_ELEMENT_NODE && node->children->type == XML_TEXT_NODE && !xmlStrcmp (node->name, (xmlChar *) "chan"))
                    {
                        if (srv->chans == NULL)
                            srv->chans = (char **) malloc (CH_BLOCK * sizeof (char *));
                        if (srv->nchan % CH_BLOCK)
                            srv->chans = (char **) realloc (srv->chans, (srv->nchan + CH_BLOCK) * sizeof (char **));

                        srv->chans[srv->nchan] = (char *) xmlStrdup (node->children->content);
                        srv->nchan += 1;
                    }

                    node = node->next;
                }
                while (node->next);

                node = node->parent;
            }
        }

        node = node->next;
    }

    return srv;
}

ircserver *
load_servers (const char * file, names * nm)
{
    ircserver * res = NULL;
    ircserver * srv = NULL;
    xmlDocPtr doc = NULL;
    xmlNodePtr node = NULL;

    LIBXML_TEST_VERSION

    if ((doc = xmlReadFile (file, NULL, 0)) == NULL)
        fprintf (stderr, "error: couldn't parse file %s\n", file);

    node = xmlDocGetRootElement (doc);

    if (xmlStrcmp (node->name, (xmlChar *) "configs"))
    {
        fprintf (stderr, "Error when loading configuration\n");
        exit (0);
    }

    node = node->children;

    while (node && xmlStrcmp (node->name, (xmlChar *) "servers"))
        node = node->next;

    node = node->children;

    do
    {
        if (node->type == XML_ELEMENT_NODE && !xmlStrcmp (node->name, (xmlChar *) "server"))
        {
            if (srv == NULL)
                srv = get_info_server (node->children, nm);
            else
            {
                srv->next = get_info_server (node->children, nm);
                srv = srv->next;
            }
            if (res == NULL)
                res = srv;
        }
    }
    while ((node = node->next));

    xmlFreeDoc (doc);
    xmlFreeNode (node);
    xmlCleanupParser();

    return res;
}

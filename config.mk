VERSION = 0.1

PREFIX = /usr/local

XML2LIB = -lxml2 -lz -lm
XML2INC = /usr/include/libxml2

INCS = -I/usr/include -I${XML2INC}
LIBS = -lpthread ${XML2LIB}

OPTIMIZE = -O2 -march=native -pipe
#Can't use -pedantic and -pedantic-errors flags because
#gcc throw errors for variadic macro
CFLAGS = ${OPTIMIZE} -std=c99 -Wall -Wextra -ansi ${INCS}
LDFLAGS = -s ${LIBS}

CC = gcc

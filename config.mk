VERSION = 0.1

XML2LIB = -lxml2 -lz -lm
XML2INC = /usr/include/libxml2

INCS = -I/usr/include -I${XML2INC}
LIBS = -lpthread ${XML2LIB} -ldl

OPTIMIZE = -O2 -march=native -pipe
#Can't use -pedantic and -pedantic-errors flags because
#gcc throw errors for variadic macro
#CFLAGS = ${OPTIMIZE} -std=c99 -Wall -Wextra -ansi ${INCS}
CFLAGS = ${OPTIMIZE} -std=c99 -Wall -Wextra -ansi -pedantic -pedantic-errors ${INCS}
LDFLAGS = -s ${LIBS}

DINLIB = -fPIC

CC = gcc

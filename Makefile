include config.mk

SOURCES =ircio.c errors.c ircparser.c server.c debug.c\
         string.c confparser.c plugins.c bbot.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = bbot

all: showoptions $(EXECUTABLE)

showoptions:
	@echo "${EXECUTABLE} build options:"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC = ${CC}"
	@echo

$(EXECUTABLE): $(OBJECTS)
	@echo "CC -o $@"
	@$(CC) $(LDFLAGS) $? -o $@

.c.o:
	@echo "CC    $<"
	@$(CC) -c $(CFLAGS) $<

clean:
	@echo "cleaning"
	@rm *.o
	@rm $(EXECUTABLE)

dist: clean
	@echo "Creating dist tarball"
	@mkdir -p ${EXECUTABLE}-${VERSION}
	@cp -R Makefile config.mk ${SOURCES} $(SOURCES:.c=.h) configs.xml\
     COPYNG README ${EXECUTABLE}-${VERSION}
	@tar -cf ${EXECUTABLE}-${VERSION}.tar ${EXECUTABLE}-${VERSION}
	@gzip ${EXECUTABLE}-${VERSION}.tar
	@rm -rf ${EXECUTABLE}-${VERSION}

install: all
	@echo "Installing executable to ${DESTDIR}${PREFIX}/bin"
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f ${EXECUTABLE} ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/${EXECUTABLE}

uninstall:
	@echo "Removing executable from ${DESTDIR}${PREFIX}/bin"
	@rm -f ${DESTDIR}${PREFIX}/bin/${EXECUTABLE}

.PHONY: clean all showoptions install uninstall dist

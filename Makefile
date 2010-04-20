include config.mk

SOURCES =ircio.c errors.c ircparser.c server.c debug.c\
         string.c confparser.c plugins.c signals.c bbot.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = bbot

all: showoptions $(EXECUTABLE)

showoptions:
	@echo "${EXECUTABLE} build options:"
	@echo "  CFLAGS = ${CFLAGS}"
	@echo "  LDFLAGS = ${LDFLAGS}"
	@echo "  CC = ${CC}"
	@echo

$(EXECUTABLE): $(OBJECTS)
	@echo "  CC -o $@"
	@$(CC) $(LDFLAGS) $? -o $@

.c.o:
	@echo "  CC    $<"
	@$(CC) -c $(CFLAGS) $< #&> /dev/null

clean:
	@echo "Cleaning"
	@rm *.o $(EXECUTABLE) *.so.0.0

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

libfiles: $(SOURCES)
	@for i in $(SOURCES); do \
	    echo "  CC    $$i"; \
	    $(CC) -c $(CFLAGS) $(DINLIB) $$i &> /dev/null; \
	done

makelib: $(OBJECTS)
	@echo "  CC -o lib${EXECUTABLE}.so.0.0"
	$(CC) -shared -Wl,-soname,lib${EXECUTABLE}.so.0 $? -o lib${EXECUTABLE}.so.0.0 -lc

lib: showoptions libfiles makelib

libinstall: lib${EXECUTABLE}.so.0.0
	@echo "Installing Library"
	@mv libbbot.so.0.0 ${DESTDIR}${PREFIX}/lib
	@ldconfig -n ${DESTDIR}${PREFIX}/lib
	@ln -sf ${DESTDIR}${PREFIX}/lib/lib${EXECUTABLE}.so.0 ${DESTDIR}${PREFIX}/lib/lib${EXECUTABLE}.so

libuninstall:
	@echo "Uninstalling library"
	@rm ${DESTDIR}${PREFIX}/lib/lib${EXECUTABLE}.so*

.PHONY: clean all showoptions install uninstall dist lib libinstall libuninstall

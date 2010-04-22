include config.mk

SOURCEDIR= sources
BUILDDIR = build
BINDIR = bin
INCLUDEDIR = includes
MODULEDIR = modules

SOURCES = $(SOURCEDIR)/ircio.c $(SOURCEDIR)/errors.c $(SOURCEDIR)/ircparser.c\
    $(SOURCEDIR)/server.c $(SOURCEDIR)/debug.c $(SOURCEDIR)/bstring.c\
    $(SOURCEDIR)/confparser.c $(SOURCEDIR)/plugins.c $(SOURCEDIR)/signals.c\
    $(SOURCEDIR)/bbot.c
OBJECTS = $(BUILDDIR)/$(EXECUTABLE)/ircio.o\
    $(BUILDDIR)/$(EXECUTABLE)/errors.o\
    $(BUILDDIR)/$(EXECUTABLE)/ircparser.o\
    $(BUILDDIR)/$(EXECUTABLE)/server.o\
    $(BUILDDIR)/$(EXECUTABLE)/debug.o\
    $(BUILDDIR)/$(EXECUTABLE)/bstring.o\
    $(BUILDDIR)/$(EXECUTABLE)/confparser.o\
    $(BUILDDIR)/$(EXECUTABLE)/plugins.o\
    $(BUILDDIR)/$(EXECUTABLE)/signals.o\
    $(BUILDDIR)/$(EXECUTABLE)/bbot.o
LIBOBJECTS = $(BUILDDIR)/lib$(EXECUTABLE)/ircio.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/errors.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/ircparser.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/server.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/debug.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/bstring.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/confparser.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/plugins.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/signals.o\
    $(BUILDDIR)/lib$(EXECUTABLE)/bbot.o
EXECUTABLE = bbot

INCS += -I${INCLUDEDIR}

all: init showoptions compilebot

init:
	@mkdir -p $(BINDIR)
	@mkdir -p $(BUILDDIR)/$(EXECUTABLE)
	@mkdir -p $(BUILDDIR)/lib$(EXECUTABLE)

showoptions:
	@echo "${EXECUTABLE} build options:"
	@echo "  CFLAGS = ${CFLAGS}"
	@echo "  LDFLAGS = ${LDFLAGS}"
	@echo "  CC = ${CC}"
	@echo

compilebot: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "  CC -o $@"
	@$(CC) $(LDFLAGS) $? -o $(BINDIR)/$@

$(OBJECTS): $(BUILDDIR)/$(EXECUTABLE)/%.o: $(SOURCEDIR)/%.c
	@echo "  CC    $<"
	@$(CC) -c $(LDFLAGS) $(INCS) $(EXTRAFLAGS) $< -o $@

clean:
	@echo "Cleaning"
	@rm -Rf bin build

dist: clean
	@echo "Creating dist tarball"
	@mkdir -p ${EXECUTABLE}-${VERSION}
	@cp -R Makefile config.mk $(SOURCEDIR) $(INCLUDEDIR) $(MODULEDIR) configs.xml\
     COPYNG README TODO ${EXECUTABLE}-${VERSION}
	@tar -cf ${EXECUTABLE}-${VERSION}.tar ${EXECUTABLE}-${VERSION}
	@gzip ${EXECUTABLE}-${VERSION}.tar
	@rm -Rf ${EXECUTABLE}-${VERSION}

install: all
	@echo "Installing executable to ${DESTDIR}${PREFIX}/bin"
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f ${BINDIR}/${EXECUTABLE} ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/${EXECUTABLE}

uninstall:
	@echo "Removing executable from ${DESTDIR}${PREFIX}/bin"
	@rm -f ${DESTDIR}${PREFIX}/bin/${EXECUTABLE}

$(LIBOBJECTS): $(BUILDDIR)/lib$(EXECUTABLE)/%.o: $(SOURCEDIR)/%.c
	@echo "  CC    $<"
	@$(CC) -c $(LDFLAGS) $(INCS) $(DINLIB) $< -o $@

makelib: $(LIBOBJECTS)
	@echo "  CC -o lib${EXECUTABLE}.so.0.0"
	$(CC) -shared -Wl,-soname,lib${EXECUTABLE}.so.0 $? -o $(BINDIR)/lib${EXECUTABLE}.so.0.0 -lc

lib: init showoptions makelib

libinstall: $(BINDIR)/lib${EXECUTABLE}.so.0.0
	@echo "Installing Library"
	@mv $(BINDIR)/libbbot.so.0.0 ${DESTDIR}${PREFIX}/lib
	@ldconfig -n ${DESTDIR}${PREFIX}/lib
	@ln -sf ${DESTDIR}${PREFIX}/lib/lib${EXECUTABLE}.so.0 ${DESTDIR}${PREFIX}/lib/lib${EXECUTABLE}.so

libuninstall:
	@echo "Uninstalling library"
	@rm ${DESTDIR}${PREFIX}/lib/lib${EXECUTABLE}.so*

.PHONY: clean all showoptions install uninstall dist lib libinstall libuninstall

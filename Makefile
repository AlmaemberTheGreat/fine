.POSIX:
.SUFFIXES:

include config.mk

SRC = fine.c cons.c
OBJ = ${SRC:.c=.o}

all: fine

${OBJ}: config.mk Makefile
fine.o: fine.c cons.h
cons.o: cons.c cons.h

fine: ${OBJ}
	@echo CC -o $@
	@${CC} ${LDFLAGS} ${OBJ} -o $@

.c.o:
	@echo CC $<
	@${CC} -c -o $@ ${CFLAGS} $<

clean:
	@echo CLEAN
	@rm -f fine ${OBJ}

install: clean all
	@echo Installing
	@cp fine.1 ${MANPREFIX}/fine.1
	@cp fine ${PREFIX}/bin/fine
	@chmod o=rx,g=rx,u=rwx ${PREFIX}/bin/fine
	@chmod o=r,g=r,u=rw ${MANPREFIX}/fine.1

dist: clean
	@mkdir fine-${VERSION}
	@cp con.c con.h config.mk Makefile fine.1 fine.c COPYING fine-${VERSION}
	@tar -cf fine-${VERSION}.tar fine-${VERSION}
	@rm -rf fine-${VERSION}
	@gzip fine-${VERSION}.tar

.SUFFIXES: .c .o

.PHONY: all clean install

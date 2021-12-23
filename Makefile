.POSIX:
.SUFFIXES:

include config.mk

SRC = fine.c con.c
OBJ = ${SRC:.c=.o}

all: fine

${OBJ}: config.mk Makefile
fine.o: fine.c con.h
con.o: con.c con.h

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

.SUFFIXES: .c .o

.PHONY: all clean install

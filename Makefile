.POSIX:
.SUFFIXES:

include config.mk

SRC = fine.c con.c
OBJ = ${SRC:.c=.o}

all: fine

${OBJ}: config.mk Makefile
fine.o: fine.c
con.o: con.c

fine: ${OBJ}
	@echo CC -o $@
	@${CC} ${LDFLAGS} ${OBJ} -o $@

.c.o:
	@echo CC $<
	@${CC} -c -o $@ ${CFLAGS} $<

clean:
	@echo CLEAN
	@rm -f fine ${OBJ}

.SUFFIXES: .c .o

.PHONY: all clean

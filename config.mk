# Basic information
VERSION=1.0.1

# Modify from here as needed to run on your system

# Commands
CC ?= cc

# Paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man/man1

# Flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_XOPEN_SOURCE=600
CFLAGS += -std=c99 -pedantic -Wall ${CPPFLAGS}
LDFLAGS +=

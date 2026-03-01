TOP=../..

CC ?= cc

LIBPATH =-L$(TOP)/lib/libnetbsd -L$(TOP)/lib/libutil
LCFLAGS ?=
LLDFLAGS ?=

CFLAGS ?=
CPPFLAGS +=-I$(TOP)/include 
LDFLAGS += $(LIBPATH)
LDADD ?=
LDLIBS += $(LDADD) -lnetbsd

PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
SBINDIR ?= $(PREFIX)/sbin
LIBDIR ?= $(PREFIX)/lib
INCDIR ?= $(PREFIX)/include/nb/

.SILENT:

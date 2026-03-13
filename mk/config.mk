TOP=../..

CC ?= gcc

LIBPATH =-L$(TOP)/lib/netbsd -L$(TOP)/lib/util
LCFLAGS ?=
LLDFLAGS ?=

CFLAGS ?= -O2
CPPFLAGS +=-I$(TOP)/include -I/usr/include
LDFLAGS += $(LIBPATH)
LDADD ?=
LDLIBS += $(LDADD) -lnetbsd

DESTDIR ?= 
PREFIX ?= usr/local
LIBDIR ?= $(PREFIX)/lib
INCDIR ?= $(PREFIX)/include/netbsd/

NETBSDSRCDIR = $(TOP)

.SILENT:

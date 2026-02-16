SUBDIRS = lib/libnetbsd lib/libutil bin 

CC=clang
AR=llvm-ar
CFLAGS=-O2
CPPFLAGS=-I$(CURDIR)/include -include compat.h
LDFLAGS=$(CURDIR)/lib/libnetbsd/libnetbsd.a
LIBUTIL=$(CURDIR)/lib/libutil/libutil.a

export CC AR CFLAGS CPPFLAGS LDFLAGS LIBUTIL

all: $(SUBDIRS)

$(SUBDIRS): 
	$(MAKE) -C $@ build
clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
.PHONY: all $(SUBDIRS) clean

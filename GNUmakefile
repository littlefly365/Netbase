LIBS=lib/libnetbsd lib/libutil 
SRC= bin/ sbin/ usr.bin/

ifeq ($(shell command -v clang 2>/dev/null),)
    CC := gcc
else
    CC := clang
endif

ifeq ($(shell command -v llvm-ar 2>/dev/null),)
    AR := ar
else
    AR := llvm-ar
endif

CFLAGS=-O2 -Wno-old-style-definition
CPPFLAGS=-I$(CURDIR)/include -include compat.h 
LDFLAGS=$(CURDIR)/lib/libnetbsd/libnetbsd.a
LIBUTIL=$(CURDIR)/lib/libutil/libutil.a 

export CC AR CFLAGS CPPFLAGS LDFLAGS LIBUTIL

all: $(SRC)

$(LIBS):
	$(MAKE) -C $@ build
$(SRC): $(LIBS)
	mkdir -p build
	$(MAKE) -C $@ build
clean:
	for dir in $(LIBS); do \
		$(MAKE) -C $$dir clean; \
	done
	for dir in $(SRC); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf build
	rm -f include/sys/extattr.h
	rm -f include/sys/cdefs.h
	rm -f include/sys/queue.h
	rm -rf include/protocols

install:
	cp build/bin/* $(DESTDIR)/$(BINDIR)
	ln -sv test $(DESTDIR)/$(BINDIR)/[
	cp build/sbin/* $(DESTDIR)/$(SBINDIR)
	cp build/usr.bin/* $(DESTDIR)/$(USRBINDIR)

.PHONY: all $(LIBS) $(SRC) clean

LIBS= lib/libnetbsd lib/libutil
SRC= sbin/ usr.bin/ usr.sbin/ extra/

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

CFLAGS=-O2 -Wno-old-style-definition -Wno-c23-extensions -Wno-visibility -fcommon
CPPFLAGS=-I$(CURDIR)/include -include compat.h -include sys/nb_cdefs.h -include nb_stdlib.h -include nb_pwd.h \
	-include nb_stdio.h -include sys/nb_param.h -include nb_regex.h -include nb_unistd.h -include sys/nb_stat.h \
	-include sys/nb_time.h 
LDFLAGS=-L$(CURDIR)/lib/libnetbsd -lnetbsd 
LIBUTIL=-L$(CURDIR)/lib/libutil -lnutil 

export CC AR CFLAGS CPPFLAGS LDFLAGS LIBUTIL

all: $(SRC)

$(LIBS):
	$(MAKE) -C lib/libnetbsd/ build
	$(MAKE) -C lib/libutil/ build
	unset CC AR CFLAGS CPPFLAGS LDFLAGS LIBUTIL
$(SRC): $(LIBS)
	mkdir -p build
	$(MAKE) -C $@ all
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

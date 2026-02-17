LIBS=lib/libnetbsd lib/libutil 
SRC= bin/ sbin/ usr.bin/
CC=clang
AR=llvm-ar
CFLAGS=-O2
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
.PHONY: all $(LIBS) $(SRC) clean

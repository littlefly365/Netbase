PROG= install

CFLAGS += -Wno-format-nonliteral -DHAVE_POSIX_SPAWN -D_EXT_SOURCE
LDFLAGS += -lmd

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -I../../usr.sbin/mtree -include sys/nb_syslimits.h -include sha256.h -include sha512.h -include nb_errc.h -c x$(PROG).c ../../usr.sbin/mtree/getid.c
$(PROG): obj
	$(CC) *.o $(LIBUTIL) $(LDFLAGS) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

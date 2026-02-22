PROG= xinstall

CFLAGS += -Wno-format-nonliteral -DHAVE_POSIX_SPAWN

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -I../../usr.sbin/mtree -c $(PROG).c ../../usr.sbin/mtree/getid.c
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) $(LIBUTIL) -o $(PROG)
clean:
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

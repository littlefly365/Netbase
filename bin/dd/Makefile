PROG= dd
CFLAGS += -DNO_CONV -DNO_MSGFMT -DNO_IOFLAG -DSMALL -fcommon
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c *.c 
$(PROG): obj
	$(CC) *.o $(LDFLAGS) $(LIBUTIL) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build

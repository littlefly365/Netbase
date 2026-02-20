PROG= sed
CFLAGS += -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=200809L
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) -include nb_errc.h -include nb_regex.h $(CFLAGS) -c *.c 
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

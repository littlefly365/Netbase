PROG= cp
CFLAGS += -D_EXT_SOURCE
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) -include sys/nb_cdefs_aout.h $(CFLAGS) -c *.c
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build

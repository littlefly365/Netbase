PROG= cut
CFLAGS += -D_EXT_SOURCE
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -include sys/nb_cdefs_aout.h -c cut.c 
$(PROG): obj
	$(CC) cut.o $(LDFLAGS) $(LIBUTIL) -o $(PROG)
clean:
	rm -rf cut.o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

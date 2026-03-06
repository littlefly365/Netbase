PROG= find
CFLAGS += -D_EXT_SOURCE -include sys/nb_syslimits.h
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c *.c 
$(PROG): obj
	$(CC) *.o $(LDFLAGS) $(LDFTS) $(LIBUTIL) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

PROG= grep

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c *.c 
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG) -lbz2 -lz
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

PROG= time
CFLAGS +=-I../../bin/
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c *.c
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

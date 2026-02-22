PROG= fmt

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -I../mail/ -c $(PROG).c ../mail/head.c
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

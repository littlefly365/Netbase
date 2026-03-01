PROG= cksum

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -include sha256.h -include sha512.h -c *.c 
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -lmd  -lcrypto -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all obj $(PROG) clean build

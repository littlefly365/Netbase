PROG= chroot

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/usr.sbin
.PHONY: all obj $(PROG) clean build

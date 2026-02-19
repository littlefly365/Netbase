PROG= pwd

all: obj $(PROG)

obj:
	$(CC) -I../../include -include compat.h $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf $(PROG) $(PROG).o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build

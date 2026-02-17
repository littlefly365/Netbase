PROG= cp
CFLAGS += 
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c *.c
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build

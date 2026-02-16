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
	mv $(PROG) ../bin-build	
.PHONY: all obj $(PROG) clean build

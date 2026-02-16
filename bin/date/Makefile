PROG= date

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(PROG).c netdate.c
$(PROG): obj
	$(CC) $(PROG).o netdate.o $(LDFLAGS) $(LIBUTIL) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../bin-build	
.PHONY: all obj $(PROG) clean build

PROG= ls

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(PROG).c cmp.c main.c print.c util.c
$(PROG): obj
	$(CC) $(PROG).o cmp.o main.o print.o util.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf *.o 
build: all
	mv $(PROG) ../../build/bin	
.PHONY: all obj $(PROG) clean build

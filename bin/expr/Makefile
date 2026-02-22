PROG= expr
CFLAGS += -fwrapv

all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c y.tab.c 
$(PROG): obj
	$(CC) y.tab.o $(LDFLAGS) -o $(PROG)
clean:
	rm -f *.o
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build

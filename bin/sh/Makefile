PROG= sh
CFLAGS += -I. -Wno-deprecated-non-prototype

all: obj $(PROG)

obj: 
	$(CC) $(CPPFLAGS) $(CFLAGS) -c *.c 
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf *.o *.out
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build 

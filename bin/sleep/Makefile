PROG= sleep

CPPFLAGS += -include stdint.h 
#CFLAGS=-D_POSIX_C_SOURCE=200809L
LDFLAGS += -lm
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build


PROG= rm
CFLAGS += -D_EXT_SOURCE 
LDFLAGS += -lfts
all: check obj $(PROG)

obj: check
	$(CC) -I../../include -include sys/nb_cdefs.h $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) -o $(PROG)
clean:
	mv rm.c.bak rm.c
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/bin
check:
	./check.sh
.PHONY: all obj $(PROG) clean build check

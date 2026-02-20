PROG= rm
CFLAGS += -D_EXT_SOURCE 
all: check obj $(PROG)

obj: check
	$(CC) -I../../include -include sys/nb_cdefs.h $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS)  $(LDFTS) -o $(PROG)
clean:
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build check

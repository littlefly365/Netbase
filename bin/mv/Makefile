PROG= mv
CFLAGS += -D_EXT_SOURCE
all: obj $(PROG)

obj: check
	$(CC) -I../../include  -include sys/nb_cdefs.h -include compat.h $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) -o $(PROG)
clean: 
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build check restore copy

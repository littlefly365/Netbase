PROG= mv
CFLAGS += -D_EXT_SOURCE
all: obj $(PROG)

obj: check
	$(CC) -I../../include  -include sys/nb_cdefs.h -include compat.h $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) -o $(PROG)
clean: 
	mv mv.c.bak mv.c
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/bin
check: check.sh copy
	./check.sh
copy:
	cp mv.c mv.c.bak
.PHONY: all obj $(PROG) clean build check restore copy

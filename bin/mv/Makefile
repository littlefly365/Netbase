PROG= mv

all: obj $(PROG)

obj: check
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) -o $(PROG)
clean: restore
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../bin-build	
check: check.sh
	./check.sh
restore:
	mv mv.c.bak mv.c
.PHONY: all obj $(PROG) clean build check restore

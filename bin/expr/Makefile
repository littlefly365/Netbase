PROG= expr
CFLAGS += -fwrapv
all: obj $(PROG)

yacc:
	byacc -d $(PROG).y
obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c y.tab.c 
$(PROG): obj
	$(CC) y.tab.o $(LDFLAGS) -o $(PROG)
clean:
	rm -rf y.tab.* 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build

PROG= ksh
CFLAGS += -I. -Wno-deprecated-non-prototype

all: emacs siglist check obj $(PROG)

obj: emacs siglist check
	$(CC) $(CPPFLAGS) $(CFLAGS) -c *.c 
$(PROG): obj
	$(CC) *.o $(LDFLAGS) -o $(PROG)
clean:
	mv trap.c.bak trap.c
	rm -rf *.o *.out
build: all
	mv $(PROG) ../../build/bin
emacs:
	./emacs-gen.sh ./emacs.c > emacs.out.tmp \
            && mv emacs.out.tmp emacs.out
siglist:
	./siglist.sh "clang -E -I. -I$(pwd) -I/usr/include" < siglist.in > siglist.out
check:
	./check.sh
.PHONY: all obj $(PROG) clean build emacs siglist

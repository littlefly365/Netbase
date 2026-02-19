PROG= chmod
all: obj $(PROG)

obj:
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LIBUTIL) -c $(PROG).c 
$(PROG): obj
	$(CC) $(PROG).o $(LDFLAGS) $(LDFTS) -o $(PROG)
clean:
	rm -rf $(PROG).o 
build: all
	mv $(PROG) ../../build/bin
.PHONY: all obj $(PROG) clean build

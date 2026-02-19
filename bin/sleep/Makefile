PROG= sleep
CFLAGS += -D_EXT_SOURCE
CPPFLAGS += -include stdint.h -include nb_locale.h -include sys/nb_cdefs_aout.h
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


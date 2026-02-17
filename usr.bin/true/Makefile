PROG = true

all: copy chmod 

chmod: copy
	chmod +x $(PROG)
copy:
	cp $(PROG).sh $(PROG)
clean:

build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all chmod copy clean build

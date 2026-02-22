PROG = c89

all: copy chmod 

chmod: copy
	chmod +x $(PROG)
copy:
	cp $(PROG).sh $(PROG)
clean:
	@echo "rm -rf c89.o"
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all chmod copy clean build

PROG = nproc

all: copy chmod 

chmod: copy
	chmod +x $(PROG)
copy:
	cp $(PROG).sh $(PROG)
clean:
	@echo "rm -rf nproc.o"
build: all
	mv $(PROG) ../../build/usr.bin
.PHONY: all chmod copy clean build

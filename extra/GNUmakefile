SUBDIRS = dir whoami nproc

all: $(SUBDIRS)

$(SUBDIRS): 
	mkdir -p ../build/usr.bin
	$(MAKE)	-C $@ build
build: $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf ../build/usr.bin
.PHONY: all $(SUBDIRS) build
	

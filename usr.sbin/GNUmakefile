SUBDIRS = chroot link

all: $(SUBDIRS)

$(SUBDIRS): 
	mkdir -p ../build/usr.sbin
	$(MAKE)	-C $@ build
build: $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf ../build/usr.sbin
.PHONY: all $(SUBDIRS) build
	

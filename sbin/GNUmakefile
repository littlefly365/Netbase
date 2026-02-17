SUBDIRS = chown

all: $(SUBDIRS)

$(SUBDIRS): 
	mkdir -p ../build/sbin
	$(MAKE)	-C $@ build
build: $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf ../build/sbin
.PHONY: all $(SUBDIRS) build
	

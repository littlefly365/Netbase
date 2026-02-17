SUBDIRS = cat chmod date domainname echo hostname ln pwd rm rmdir sleep sync date mkdir mv

all: $(SUBDIRS)

$(SUBDIRS): 
	mkdir -p bin-build
	$(MAKE)	-C $@ build
build: $(SUBDIRS)

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	rm -rf bin-build
.PHONY: all $(SUBDIRS) build
	
